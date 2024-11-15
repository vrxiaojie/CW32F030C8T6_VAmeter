/**
* @file         mqtt.c
* @brief  		MQTT服务相关操作的源文件
* @author		VRxiaojie
* @date		    2024-08-11
* @version	    v1.0
* @note 	 	哔哩哔哩	:		VR小杰		https://space.bilibili.com/11526854 <br>
* 				嘉立创开源广场:	vrxiaojie	https://oshwhub.com/vrxiaojie/	
*/

#include "mqtt.h"

uint8_t mqtt_status = 0 ;  ///<MQTT连接状态

/**
* @brief		MQTT设置
* @param		无
* @return		无	
*/

void MQTT_Setup()
{
	ESP8266_Clear();
    uint8_t t = 1;
    if(WIFI_Status==0)
    {
        Gui_DrawFont_GBK16(8,72,RED,YELLOW,"WIFI Not Connected");
        delay_ms(300);
		Show_Main_Menu();
    }
    else
    {
        Lcd_Clear(BLACK);
        Gui_DrawFont_GBK16(16,0,BLACK,YELLOW,"Connect to MQTT");
        if(ESP8266_SendCmd((uint8_t*)strcat_new(USERCFG,"\r\n"),(uint8_t*)"OK"))
            Gui_DrawFont_GBK16(0,16,GREEN,BLACK,"User Config OK");
        else
        {
            Gui_DrawFont_GBK16(0,16,RED,BLACK,"Err:User Config");
            t = 0;
        }
        delay_ms(100);

        if(t==1&&ESP8266_SendCmd((uint8_t*)strcat_new(CLIENTID,"\r\n"),(uint8_t*)"OK"))
            Gui_DrawFont_GBK16(0,32,GREEN,BLACK,"ClientID Config OK");
        else
        {
            Gui_DrawFont_GBK16(0,32,RED,BLACK,"Err:ClientID");
            t=0;
        }
        delay_ms(100);

        if(t==1&&ESP8266_SendCmd((uint8_t*)strcat_new(MQTTCONN,"\r\n"),(uint8_t*)"OK"))
            Gui_DrawFont_GBK16(0,48,GREEN,BLACK,"MQTT Connected");
        else
        {
            Gui_DrawFont_GBK16(0,48,RED,BLACK,"连接失败,检查用户名");
			Gui_DrawFont_GBK16(0,64,RED,BLACK,"或密码或服务器ip");
			Gui_DrawFont_GBK16(0,80,RED,BLACK,"和端口是否准确");
            t=0;
			delay_ms(1000);
        }
        delay_ms(100);
      
        if(t)
		{
			FUN[3][0] = MQTT_Disconnect;
			sub_menu_button[3][0]="断开MQTT  ";
			mqtt_status = 1;
		}
		delay_ms(150);
		Lcd_Clear(BLACK);
		Show_Main_Menu();
		in_progress_flag = 0;
    }
}

/**
* @brief		断开MQTT连接
* @param		无
* @return		无	
*/
void MQTT_Disconnect()
{
	Lcd_Clear(BLACK);
	Gui_DrawFont_GBK16(20,56,BLACK,YELLOW,"处理中...");
	if(ESP8266_SendCmd((uint8_t*)"AT+MQTTCLEAN=0\r\n",(uint8_t*)"OK"))
	{
		Gui_DrawFont_GBK16(20,72,BLACK,GREEN,"MQTT Disconnected");
		FUN[3][0] = MQTT_Setup;
		sub_menu_button[3][0]="连接MQTT  ";
		mqtt_status = 0;
	}
	else
	{
		Gui_DrawFont_GBK16(20,72,BLACK,RED,"Failed");
	}
}

/**
* @brief		在MQTT指定话题发布消息
* @param		data 要发布的消息
* @return		无	
*/

void MQTT_Publish(char *data)
{
	if(mqtt_status==1)
	{
		char buffer1[400] = {0};
		strcpy(buffer1,"AT+MQTTPUB=0,\"");
		strcat(buffer1,MQTT_TOPIC);
		strcat(buffer1,"\",\"");
		strcat(buffer1,data);
		strcat(buffer1,"\",0,0\r\n");
		ESP8266_SendCmd((uint8_t*)buffer1,(uint8_t*)"OK");
		free(buffer1);
	}
}
