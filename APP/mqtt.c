/*
 * ���������˺�		: 	VRС��      	https://space.bilibili.com/11526854
 * ��������Դ�㳡	: 	vrxiaojie   https://oshwhub.com/vrxiaojie/
 *
 *   Date           Author
 * 2024-08-10     VRxiaojie
 */
#include "mqtt.h"

uint8_t mqtt_status = 0 ;

/**************************************************************************************
	�������ƣ�	MQTT_Setup

	�������ܣ�	MQTT����

	��ڲ�����	��

	���ز�����	��
**************************************************************************************/
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
            Gui_DrawFont_GBK16(0,48,RED,BLACK,"Err:Connection");
            t=0;
        }
        delay_ms(100);
      
        if(t)
		{
			FUN[3][0] = MQTT_Disconnect;
			sub_menu_button[3][0]="�Ͽ�MQTT  ";
			mqtt_status = 1;
		}
		delay_ms(150);
		Lcd_Clear(BLACK);
		Show_Main_Menu();
		in_progress_flag = 0;
    }
}

/**************************************************************************************
	�������ƣ�	MQTT_Disconnect

	�������ܣ�	MQTT�Ͽ�����

	��ڲ�����	��

	���ز�����	��
**************************************************************************************/
void MQTT_Disconnect()
{
	Lcd_Clear(BLACK);
	Gui_DrawFont_GBK16(20,56,BLACK,YELLOW,"������...");
	if(ESP8266_SendCmd((uint8_t*)"AT+MQTTCLEAN=0\r\n",(uint8_t*)"OK"))
	{
		Gui_DrawFont_GBK16(20,72,BLACK,GREEN,"MQTT Disconnected");
		FUN[3][0] = MQTT_Setup;
		sub_menu_button[3][0]="����MQTT  ";
		mqtt_status = 0;
	}
	else
	{
		Gui_DrawFont_GBK16(20,72,BLACK,RED,"Failed");
	}
}

/**************************************************************************************
	�������ƣ�	MQTT_Publish

	�������ܣ�	MQTT������Ϣ

	��ڲ�����	data����Ҫ��������Ϣ

	���ز�����	��
**************************************************************************************/
void MQTT_Publish(char *data)
{
	if(mqtt_status==1)
	{
		char buffer[400] = {0};
		strcpy(buffer,"AT+MQTTPUB=0,\"");
		strcat(buffer,MQTT_TOPIC);
		strcat(buffer,"\",\"");
		strcat(buffer,data);
		strcat(buffer,"\",0,0\r\n");
		ESP8266_SendCmd((uint8_t*)buffer,(uint8_t*)"OK");
		free(buffer);
	}
}
