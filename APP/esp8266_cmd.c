/**
* @file         esp8266_cmd.c
* @brief		ESP8266��ز���Դ�ļ�
* @author		VRxiaojie
* @date		    2024-08-11
* @version	    v1.0
* @note 	 	��������	:		VRС��		https://space.bilibili.com/11526854 <br>
* 				��������Դ�㳡:	vrxiaojie	https://oshwhub.com/vrxiaojie/	
*/

#include "board.h"
#include "Lcd_Driver.h"
#include "GUI.h"
#include "esp8266_cmd.h"
#include "menu.h"
#include "main.h"
#include "mqtt.h"
#include "tim.h"

//Ԥ�������
uint8_t esp8266_buf[1000],WIFI_Status=0,retry_cnt = 0,connect_retry_cnt = 0;
uint16_t esp8266_cnt, esp8266_cntPre;
//��ʼ��ESP8266״̬��ʶ������ESP8266�Ƿ񿪻����Ƿ����ӵ�CW32��
uint8_t ESP8266_Status = 0;
uint8_t ESP8266_Last_Status = 0;


/**
* @brief		USART�����ַ���
* @param		USARTx UART���š����õ��У�CW_UART1��CW_UART2��CW_UART3
* @param		DataString Ҫ���͵��ַ�������
* @return		��	
*/

void USART_SendString(UART_TypeDef* USARTx, uint8_t *DataString)
{
    int i = 0;
    USART_ClearFlag(USARTx,USART_FLAG_TC);										//�����ַ�ǰ��ձ�־λ������ȱʧ�ַ����ĵ�һ���ַ���
    while(DataString[i] != '\0')												//�ַ���������
    {
        USART_SendData(USARTx,DataString[i]);									//ÿ�η����ַ�����һ���ַ�
        while(USART_GetFlagStatus(USARTx,USART_FLAG_TC) == 0);					//�ȴ����ݷ��ͳɹ�
        USART_ClearFlag(USARTx,USART_FLAG_TC);									//�����ַ�����ձ�־λ
        i++;
    }
}

/**
* @brief		���ESP8266 UART���ջ���
* @param		��
* @return		��	
*/

void ESP8266_Clear(void)
{

    memset(esp8266_buf, 0, sizeof(esp8266_buf));
    esp8266_cnt = 0;

}

/**
* @brief		�ȴ�ESP8266�������
* @param		��
* @return		1 �������
* @return		0 ���ճ�ʱδ���
*/

uint8_t ESP8266_WaitRecive(void)
{

    if(esp8266_cnt == 0) 							//������ռ���Ϊ0 ��˵��û�д��ڽ��������У�����ֱ����������������
        return 0;

    if(esp8266_cnt == esp8266_cntPre)				//�����һ�ε�ֵ�������ͬ����˵���������
    {
        esp8266_cnt = 0;							//��0���ռ���

        return 1;								//���ؽ�����ɱ�־
    }

    esp8266_cntPre = esp8266_cnt;					//��Ϊ��ͬ

    return 0;								//���ؽ���δ��ɱ�־

}

/**
* @brief		��������
* @param		cmd ����
* @param		res ��Ҫ���ķ���ָ��
* @return		1 �ɹ�
* @return		0 ʧ��
*/

uint8_t ESP8266_SendCmd(uint8_t *cmd, uint8_t *res)
{

    uint16_t timeOut = 3000;
	ESP8266_Clear();
    USART_SendString(CW_UART1, (uint8_t *)cmd);

    while(timeOut--)
    {
        if(ESP8266_WaitRecive() == 1)							//����յ�����
        {
            if(strstr((const char *)esp8266_buf,(const char *)res) != NULL)		//����������ؼ���
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
* @brief		��ѯ������Ϣ
* @param		��
* @return		��
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
        Gui_DrawFont_GBK16(48,0,LIGHTBLUE,BLACK,"������Ϣ");
        //�жϲ�ѯ�����Ƿ���ķ����������Ϣ
        if(strstr((const char*)esp8266_buf,(const char*)"ip")==NULL)
        {
            Gui_DrawFont_GBK16(25,55,GREEN,BLACK,"������...");
            delay_ms(200);
            if(retry_cnt>=3)
            {
                retry_cnt=0;
                Gui_DrawFont_GBK16(25,55,RED,BLACK,"��ѯʧ��");
                delay_ms(300);
                return ;
            }
            retry_cnt++;
            Network_info();
            return ;
        }

        //������ʾ�ַ����е�IP
        char * token = strtok((char*)esp8266_buf,s);
        token = strtok(NULL,s);
        if(strcmp("0.0.0.0",token)==0 || strlen(token)==0) //δ��ȡ��IPʱ�ݹ��������
        {
            Network_info();
            if(retry_cnt>=3)
            {
                retry_cnt=0;
                Gui_DrawFont_GBK16(25,55,RED,YELLOW,"��ѯʧ��");
                delay_ms(300);
                return ;
            }
            retry_cnt++;
            Network_info();
            return ;
        }
        Gui_DrawFont_GBK16(0,16,WHITE,BLACK,strcat_new(" IP :",token));
        //������ʾ�ַ����е�����
        token = strtok(NULL,s);
        token = strtok(NULL,s);
        Gui_DrawFont_GBK16(0,32,WHITE,BLACK,strcat_new("����:",token));
        //������ʾ�ַ����е���������
        token = strtok(NULL,s);
        token = strtok(NULL,s);
        Gui_DrawFont_GBK16(0,48,WHITE,BLACK,strcat_new("����:",token));

        ESP8266_Clear();
    }
}

/**
* @brief		����WIFI
* @param		��
* @return		��
*/

void Connect_WIFI() {
	GPIO_WritePin(ESP8266_EN_PORT,ESP8266_EN_PIN,GPIO_Pin_SET); //����ESP8266
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
			sub_menu_button[2][0]="�Ͽ�WIFI  ";
			
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
* @brief		�Ͽ�WIFI
* @param		��
* @return		��
*/

void Disconnect_WIFI()
{
    if(WIFI_Status==1)
    {
        if(ESP8266_SendCmd((uint8_t *)"AT+CWQAP\r\n",(uint8_t *)"OK"))
        {
            Gui_DrawFont_GBK16(0,32,GREEN,BLACK,"WIFI Disconnected!");
			GPIO_WritePin(ESP8266_EN_PORT,ESP8266_EN_PIN,GPIO_Pin_RESET); //ֱ�ӹر�ESP8266
            WIFI_Status=0;
			mqtt_status = 0;
			sub_menu_button[2][0]="����WIFI  ";
			sub_menu_button[3][0]="����MQTT  ";
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
* @brief		����ESP8266
* @param		echo ѡ�����ò����Ƿ���ʾ����Ļ 1-��ʾ 0-����ʾ
* @return		��
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
		Gui_DrawFont_GBK16(20,56,BLACK,YELLOW,"������...");
    if(ESP8266_SendCmd((uint8_t *)"AT+RST\r\n",(uint8_t *)"ready"))
    {
        //�ر��Զ�����WIFI������ΪWIFI OFFģʽ
        USART_SendString(CW_UART1,(uint8_t*)"AT+CWMODE=0\r\n");
		USART_SendString(CW_UART1,(uint8_t*)"AT+CWAUTOCONN=0\r\n");
		if(echo)
			Gui_DrawFont_GBK16(20,72,BLACK,GREEN,"Reset \nSuccess!");
        WIFI_Status = 0;
		FUN[2][0] = Connect_WIFI;
		FUN[3][0] = MQTT_Setup;
		sub_menu_button[2][0]="����WIFI  ";
		sub_menu_button[3][0]="����MQTT  ";
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
