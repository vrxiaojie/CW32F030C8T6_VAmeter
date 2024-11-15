/**
* @file         esp8266_cmd.c
* @brief		ESP8266相关操作源文件
* @author		VRxiaojie
* @date		    2024-08-11
* @version	    v1.0
* @note 	 	哔哩哔哩	:		VR小杰		https://space.bilibili.com/11526854 <br>
* 				嘉立创开源广场:	vrxiaojie	https://oshwhub.com/vrxiaojie/	
*/

#include "board.h"
#include "Lcd_Driver.h"
#include "GUI.h"
#include "esp8266_cmd.h"
#include "menu.h"
#include "main.h"
#include "mqtt.h"
#include "tim.h"

//预定义变量
uint8_t esp8266_buf[1000],WIFI_Status=0,retry_cnt = 0,connect_retry_cnt = 0;
uint16_t esp8266_cnt, esp8266_cntPre;
//初始化ESP8266状态标识变量（ESP8266是否开机、是否连接到CW32）
uint8_t ESP8266_Status = 0;
uint8_t ESP8266_Last_Status = 0;


/**
* @brief		USART发送字符串
* @param		USARTx UART引脚。可用的有：CW_UART1，CW_UART2，CW_UART3
* @param		DataString 要发送的字符串数组
* @return		无	
*/

void USART_SendString(UART_TypeDef* USARTx, uint8_t *DataString)
{
    int i = 0;
    USART_ClearFlag(USARTx,USART_FLAG_TC);										//发送字符前清空标志位（否则缺失字符串的第一个字符）
    while(DataString[i] != '\0')												//字符串结束符
    {
        USART_SendData(USARTx,DataString[i]);									//每次发送字符串的一个字符
        while(USART_GetFlagStatus(USARTx,USART_FLAG_TC) == 0);					//等待数据发送成功
        USART_ClearFlag(USARTx,USART_FLAG_TC);									//发送字符后清空标志位
        i++;
    }
}

/**
* @brief		清空ESP8266 UART接收缓存
* @param		无
* @return		无	
*/

void ESP8266_Clear(void)
{

    memset(esp8266_buf, 0, sizeof(esp8266_buf));
    esp8266_cnt = 0;

}

/**
* @brief		等待ESP8266接收完成
* @param		无
* @return		1 接收完成
* @return		0 接收超时未完成
*/

uint8_t ESP8266_WaitRecive(void)
{

    if(esp8266_cnt == 0) 							//如果接收计数为0 则说明没有处于接收数据中，所以直接跳出，结束函数
        return 0;

    if(esp8266_cnt == esp8266_cntPre)				//如果上一次的值和这次相同，则说明接收完毕
    {
        esp8266_cnt = 0;							//清0接收计数

        return 1;								//返回接收完成标志
    }

    esp8266_cntPre = esp8266_cnt;					//置为相同

    return 0;								//返回接收未完成标志

}

/**
* @brief		发送命令
* @param		cmd 命令
* @param		res 需要检查的返回指令
* @return		1 成功
* @return		0 失败
*/

uint8_t ESP8266_SendCmd(uint8_t *cmd, uint8_t *res)
{

    uint16_t timeOut = 3000;
	ESP8266_Clear();
    USART_SendString(CW_UART1, (uint8_t *)cmd);

    while(timeOut--)
    {
        if(ESP8266_WaitRecive() == 1)							//如果收到数据
        {
            if(strstr((const char *)esp8266_buf,(const char *)res) != NULL)		//如果检索到关键词
            {
				ESP8266_Clear();
                return 1;
            }
        }

        delay_ms(1);
    }
    return 0;
}

/**
* @brief		查询网络信息
* @param		无
* @return		无
*/

void Network_info() {
	if(ESP8266_Status == 0)
	{
		Gui_DrawFont_GBK16(8,72,RED,YELLOW,"ESP8266 Not Found");
		delay_ms(250);
		return ;
	}
    ESP8266_Clear();
    if(WIFI_Status==0) {
        Gui_DrawFont_GBK16(8,72,RED,YELLOW,"WIFI Not Connected");
		delay_ms(200);
		Show_Main_Menu();
		in_progress_flag = 0;
        return ;
    }
    if(ESP8266_SendCmd((uint8_t*)"AT+CIPSTA?\r\n",(uint8_t*)"OK")) {
        Lcd_Clear(BLACK);
        const char s[2]="\"";
        Gui_DrawFont_GBK16(48,0,LIGHTBLUE,BLACK,"网络信息");
        //判断查询命令是否真的返回了相关信息
        if(strstr((const char*)esp8266_buf,(const char*)"ip")==NULL)
        {
            Gui_DrawFont_GBK16(25,55,GREEN,BLACK,"处理中...");
            delay_ms(200);
            if(retry_cnt>=3)
            {
                retry_cnt=0;
                Gui_DrawFont_GBK16(25,55,RED,BLACK,"查询失败");
                delay_ms(300);
                return ;
            }
            retry_cnt++;
            Network_info();
            return ;
        }

        //处理并显示字符串中的IP
        char * token = strtok((char*)esp8266_buf,s);
        token = strtok(NULL,s);
        if(strcmp("0.0.0.0",token)==0 || strlen(token)==0) //未获取到IP时递归调用重试
        {
            Network_info();
            if(retry_cnt>=3)
            {
                retry_cnt=0;
                Gui_DrawFont_GBK16(25,55,RED,YELLOW,"查询失败");
                delay_ms(300);
                return ;
            }
            retry_cnt++;
            Network_info();
            return ;
        }
        Gui_DrawFont_GBK16(0,16,WHITE,BLACK,strcat_new(" IP :",token));
        //处理并显示字符串中的网关
        token = strtok(NULL,s);
        token = strtok(NULL,s);
        Gui_DrawFont_GBK16(0,32,WHITE,BLACK,strcat_new("网关:",token));
        //处理并显示字符串中的子网掩码
        token = strtok(NULL,s);
        token = strtok(NULL,s);
        Gui_DrawFont_GBK16(0,48,WHITE,BLACK,strcat_new("掩码:",token));

        ESP8266_Clear();
    }
}

/**
* @brief		连接WIFI
* @param		无
* @return		无
*/

void Connect_WIFI() {
	GPIO_WritePin(ESP8266_EN_PORT,ESP8266_EN_PIN,GPIO_Pin_SET); //开启ESP8266
	delay_ms(100);
	if(GPIO_ReadPin(ESP8266_PORT,ESP8266_PIN)==GPIO_Pin_SET)
	{
		ESP8266_Status = 1;
	}
	if(ESP8266_Status == 0)
	{
		Gui_DrawFont_GBK16(8,72,RED,YELLOW,"ESP8266 Not Found");
		delay_ms(250);
		return ;
	}
    if(WIFI_Status==0) {
		delay_ms(200);
        char *t = malloc(100);
        strcat(t,"AT+CWJAP=\"");
        strcat(t,WIFI_SSID);
        strcat(t,"\",\"");
        strcat(t,WIFI_PASSWORD);
        strcat(t,"\"\r\n");
		
        Gui_DrawFont_GBK16(0,0,WHITE,BLACK,"WIFI Connecting...");
		ESP8266_SendCmd((uint8_t *)"AT+CWMODE=1,0\r\n",(uint8_t *)"OK");
        if(ESP8266_SendCmd((uint8_t *)t,(uint8_t *)"CONNECTED"))
        {
            WIFI_Status = 1;
			FUN[2][0] = Disconnect_WIFI;
			sub_menu_button[2][0]="断开WIFI  ";
			
            Gui_DrawFont_GBK16(0,16,GREEN,BLACK,"WIFI Connected!");
            Gui_DrawFont_GBK16(0,32,WHITE,BLACK,strcat_new("SSID:",WIFI_SSID));
			ESP8266_Last_Status = 1;
            free(t);
            connect_retry_cnt = 0;
			delay_ms(200);
			Lcd_Clear(BLACK);
			Show_Main_Menu();
			in_progress_flag = 0;
            return ;
        }
        else
        {
            free(t);
            WIFI_Status = 0;
            connect_retry_cnt++;
            switch(connect_retry_cnt)
            {
            case 1:
                Gui_DrawFont_GBK16(0,16,RED,BLACK,"Retrying...[1]");
                Connect_WIFI();
                break;
            case 2:
                Gui_DrawFont_GBK16(0,16,RED,BLACK,"Retrying...[2]");
                Connect_WIFI();
                break;
            default:
                connect_retry_cnt=0;
				ESP8266_SendCmd((uint8_t *)"AT+CWMODE=0,0\r\n",(uint8_t *)"OK");
                Gui_DrawFont_GBK16(0,16,RED,BLACK,"WIFI Not Connected!");
				delay_ms(200);
				Lcd_Clear(BLACK);
				Show_Main_Menu();
				in_progress_flag = 0;
                break;
            }
            return ;
        }
    }
}

/**
* @brief		断开WIFI
* @param		无
* @return		无
*/

void Disconnect_WIFI()
{
    if(WIFI_Status==1)
    {
        if(ESP8266_SendCmd((uint8_t *)"AT+CWQAP\r\n",(uint8_t *)"OK"))
        {
            Gui_DrawFont_GBK16(0,32,GREEN,BLACK,"WIFI Disconnected!");
			GPIO_WritePin(ESP8266_EN_PORT,ESP8266_EN_PIN,GPIO_Pin_RESET); //直接关闭ESP8266
            WIFI_Status=0;
			mqtt_status = 0;
			sub_menu_button[2][0]="连接WIFI  ";
			sub_menu_button[3][0]="连接MQTT  ";
			FUN[2][0] = Connect_WIFI;
			FUN[3][0] = MQTT_Setup;
			ESP8266_Last_Status = 0;
			delay_ms(200);
			Lcd_Clear(BLACK);
			Show_Main_Menu();
			in_progress_flag = 0;
        }
        else
            Gui_DrawFont_GBK16(0,32,RED,BLACK,"WIFI Disconnect Failed!");
    }
    return ;
}

/**
* @brief		重置ESP8266
* @param		echo 选择重置操作是否显示于屏幕 1-显示 0-不显示
* @return		无
*/
void ESP8266_Reset(uint8_t echo)
{
	if(ESP8266_Status == 0)
	{
		if (echo){
			Gui_DrawFont_GBK16(8,72,RED,YELLOW,"ESP8266 Not Found");
		}
		delay_ms(250);
		Lcd_Clear(BLACK);
		Show_Main_Menu();
		in_progress_flag = 0;
		return ;
	}
	if(echo)
		Gui_DrawFont_GBK16(20,56,BLACK,YELLOW,"处理中...");
    if(ESP8266_SendCmd((uint8_t *)"AT+RST\r\n",(uint8_t *)"ready"))
    {
        //关闭自动连接WIFI，设置为WIFI OFF模式
        USART_SendString(CW_UART1,(uint8_t*)"AT+CWMODE=0\r\n");
		USART_SendString(CW_UART1,(uint8_t*)"AT+CWAUTOCONN=0\r\n");
		if(echo)
			Gui_DrawFont_GBK16(20,72,BLACK,GREEN,"Reset \nSuccess!");
        WIFI_Status = 0;
		FUN[2][0] = Connect_WIFI;
		FUN[3][0] = MQTT_Setup;
		sub_menu_button[2][0]="连接WIFI  ";
		sub_menu_button[3][0]="连接MQTT  ";
		delay_ms(200);
		Lcd_Clear(BLACK);
		Show_Main_Menu();
		in_progress_flag = 0;
    }
    else
    {
		if(echo)
			Gui_DrawFont_GBK16(20,72,BLACK,RED,"Reset Failed!");
		delay_ms(200);
		Show_Main_Menu();
		in_progress_flag = 0;
    }
}
