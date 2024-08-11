<<<<<<< HEAD
/**
* @file         menu.c
* @brief		�˵���غ���Դ�ļ�
* @author		VRxiaojie
* @date		    2024-08-11
* @version	    v1.0
* @note 	 	��������	:		VRС��		https://space.bilibili.com/11526854 <br>
* 				��������Դ�㳡:	vrxiaojie	https://oshwhub.com/vrxiaojie/	
*/

=======
/*
 * ���������˺�		: 	VRС��      	https://space.bilibili.com/11526854
 * ��������Դ�㳡	: 	vrxiaojie   https://oshwhub.com/vrxiaojie/
 *
 *   Date           Author
 * 2024-08-10     VRxiaojie
 */
>>>>>>> 0dfd7a7e8c0db99ec07f30fe2edd2e84eaf20952
#include "menu.h"
#include "GUI.h"
#include "Lcd_Driver.h"
#include "esp8266_cmd.h"
#include "main.h"
#include "mqtt.h"
#include "sys.h"
#include "adc.h"
#include "va_meter.h"
#include "sht40.h"
#include <math.h>

uint8_t main_menu_index = 0;
uint8_t sub_menu_index = 0;
uint8_t in_progress_flag = 0;
uint8_t in_sub_menu_flag = 0;


/*��Ÿ������к���������*/
void (*FUN[5][5])() = {
{Launch_VA_Meter,Launch_VA_Meter},//��ѹ����
{SHT40_Show_Temp_Humid},//��ʪ�ȼ�
{Connect_WIFI,Network_info},//WIFI
{MQTT_Setup},//MQTT
{System_Info,ESP8266_Reset,MCU_Sleep,Brightness_Setup,Enable_or_Disable_ESP}};//ϵͳ����
//������˵���ť����
char* main_menu_button[] = {"��ѹ����","��ʪ�ȼ�","WIFI    ","MQTT    ","ϵͳ����"};
//��Ŵμ��˵���ť����
char* sub_menu_button[5][5]={{"ͼ��ģʽ  ","����ģʽ  ","          ","          ","          "},
{"��ʪ��    ","          ","          ","          ","          "},
{"����WIFI  ","������Ϣ  ","          ","          ","          "},
{"����MQTT  ","          ","          ","          ","          "},
{"ϵͳ����  ","����ESP   ","����      ","��������  ","����ESP8266 "}};
uint8_t sub_menu_maxidx[5] = {SUB_MENU_0_INDEX,SUB_MENU_1_INDEX,SUB_MENU_2_INDEX,SUB_MENU_3_INDEX,SUB_MENU_4_INDEX};

<<<<<<< HEAD
/**
* @brief		��ʾ״̬��
* @param		��
* @return		��	
*/

=======
/**************************************************************************************
	�������ƣ�	Show_Status_Bar

	�������ܣ�	��ʾ״̬��

	��ڲ�����	��

	���ز�����	��
**************************************************************************************/
>>>>>>> 0dfd7a7e8c0db99ec07f30fe2edd2e84eaf20952
void Show_Status_Bar()
{
	//�����ŵ�ص���
	uint16_t bat_volume = Battery_Volume();
	if(bat_volume>=61 && bat_volume<=100)
	{
		Gui_DrawFont_Num16(0,0,GREEN,BLACK,bat_volume/10);
		Gui_DrawFont_Num16(8,0,GREEN,BLACK,bat_volume%10);
	}
	else if(bat_volume>=21 && bat_volume<=60)
	{
		Gui_DrawFont_Num16(0,0,YELLOW,BLACK,bat_volume/10);
		Gui_DrawFont_Num16(8,0,YELLOW,BLACK,bat_volume%10);
	}
	else if(bat_volume>0&& bat_volume<=20)
	{
		Gui_DrawFont_Num16(0,0,RED,BLACK,bat_volume/10);
		Gui_DrawFont_Num16(8,0,RED,BLACK,bat_volume%10);
	}
	Gui_DrawFont_GBK16(16,0,WHITE,BLACK,"%");
	
    //������һ����WIFI״̬
    switch(WIFI_Status)
    {
    case 0:
        showimage_16(WIFI_Disconnected_Icon,144,0);
        mqtt_status = 0;
        break;
    case 1:
        showimage_16(WIFI_Connected_Icon,144,0);
        break;
    }

    //�����ڶ�����ESP8266����״̬
    switch(ESP8266_Status)
    {
    case 0 :
        showimage_16(ESP8266_ERROR,124,0);
        break;
    case 1 :
        showimage_16(ESP8266_OK,124,0);
        break;
    }
    //������������MQTT����״̬
    switch(mqtt_status)
    {
    case 0:
        showimage_16(Mqtt_Error_Icon,104,0);
        break;
    case 1:
        showimage_16(Mqtt_OK_Icon,104,0);
        break;
    }
}
<<<<<<< HEAD
/**
* @brief		��ʾ���˵�
* @param		��
* @return		��	
*/

=======

/**************************************************************************************
	�������ƣ�	Show_Main_Menu

	�������ܣ�	��ʾ���˵�

	��ڲ�����	��

	���ز�����	��
**************************************************************************************/
>>>>>>> 0dfd7a7e8c0db99ec07f30fe2edd2e84eaf20952
void Show_Main_Menu()
{
    //���ָ���
    Gui_DrawLine(0,20,160,20,GREEN);
    Gui_DrawLine(80,20,80,128,GREEN);
    //��ʾ״̬��
    Show_Status_Bar();
    //��Ⱦ���˵�
    for(uint8_t i = 0; i<5; i++)
    {
        uint8_t x = 0,y = 22+i*20;
        Gui_DrawFont_GBK16(x,y,WHITE,BLACK,main_menu_button[i]);
    }
    Choose_Main_Function();  //����ѡ�����˵�
	Show_Sub_Menu(); //ͬʱ��ʾ��ǰ�μ��˵�
	if(in_sub_menu_flag == 1) //���ڴμ��˵��������ѡ�дμ��˵��Ĺ���
		Choose_Sub_Function();
}

<<<<<<< HEAD
/**
* @brief		��ʾ�μ��˵�
* @param		��
* @return		��	
*/

=======
/**************************************************************************************
	�������ƣ�	Show_Sub_Menu

	�������ܣ�	��ʾ�μ��˵�

	��ڲ�����	��

	���ز�����	��
**************************************************************************************/
>>>>>>> 0dfd7a7e8c0db99ec07f30fe2edd2e84eaf20952
void Show_Sub_Menu()
{
	Lcd_Part_Clear(81,22,160,128,BLACK);
	for(uint8_t i = 0; i<=sub_menu_maxidx[main_menu_index]; i++)
    {
        uint8_t x = 81,y = 22+i*20;
        Gui_DrawFont_GBK16(x,y,WHITE,BLACK,sub_menu_button[main_menu_index][i]);
    }
}

<<<<<<< HEAD
/**
* @brief		�μ��˵���ť��ѡ��
* @param		��
* @return		��	
*/

=======

/**************************************************************************************
	�������ƣ�	Choose_Sub_Function

	�������ܣ�	�μ��˵���ť��ѡ��

	��ڲ�����	��

	���ز�����	��
**************************************************************************************/
>>>>>>> 0dfd7a7e8c0db99ec07f30fe2edd2e84eaf20952
void Choose_Sub_Function()
{
	uint8_t x = 81,y=40;
    Gui_DrawLine(x,y+sub_menu_index*20,x+75,y+sub_menu_index*20,ORANGE);
}
<<<<<<< HEAD

/**
* @brief		���˵���ť��ѡ��
* @param		��
* @return		��	
*/

=======
/**************************************************************************************
	�������ƣ�	Choose_Main_Function

	�������ܣ�	���˵���ť��ѡ��

	��ڲ�����	��

	���ز�����	��
**************************************************************************************/
>>>>>>> 0dfd7a7e8c0db99ec07f30fe2edd2e84eaf20952
void Choose_Main_Function()
{
    uint8_t x = 0,y=40;
	Gui_DrawLine(x,y+main_menu_index*20,x+75,y+main_menu_index*20,LIGHTBLUE);
}

<<<<<<< HEAD
/**
* @brief		�μ��˵���ť�ָ�δ��ѡ��״̬
* @param		idx ֮ǰ��ѡ�еĴμ��˵�����
* @return		��	
*/

=======

/**************************************************************************************
	�������ƣ�	Restore_Sub_Menu_Button

	�������ܣ�	�μ��˵���ť�ָ�δ��ѡ��״̬

	��ڲ�����	��

	���ز�����	��
**************************************************************************************/
>>>>>>> 0dfd7a7e8c0db99ec07f30fe2edd2e84eaf20952
void Restore_Sub_Menu_Button(uint8_t idx)
{
	uint8_t x = 81,y = 40;
	Gui_DrawLine(x,y+idx*20,x+75,y+idx*20,BLACK);
}

<<<<<<< HEAD
/**
* @brief		���˵���ť�ָ�δ��ѡ��״̬
* @param		idx ֮ǰ��ѡ�е������˵�����
* @return		��	
*/

=======
/**************************************************************************************
	�������ƣ�	Restore_Main_Menu_Button

	�������ܣ�	���˵���ť�ָ�δ��ѡ��״̬

	��ڲ�����	��

	���ز�����	��
**************************************************************************************/
>>>>>>> 0dfd7a7e8c0db99ec07f30fe2edd2e84eaf20952
void Restore_Main_Menu_Button(uint8_t idx)
{
	uint8_t x = 0,y = 40;
	Gui_DrawLine(x,y+idx*20,x+75,y+idx*20,BLACK);
}

<<<<<<< HEAD
/**
* @brief		ִ�ж�Ӧ�Ĺ��ܺ���
* @param		��
* @return		��	
*/

=======
/**************************************************************************************
	�������ƣ�	Run_Function

	�������ܣ�	ִ�ж�Ӧ�Ĺ��ܺ���

	��ڲ�����	��

	���ز�����	��
**************************************************************************************/
>>>>>>> 0dfd7a7e8c0db99ec07f30fe2edd2e84eaf20952
void Run_Function()
{
    in_progress_flag = 1;
	Lcd_Clear(BLACK);
	if(main_menu_index==0)  //��Ե�ѹ������˵��������ã����ڴ���
	{
		FUN[main_menu_index][sub_menu_index](sub_menu_index);
	}
		
	else
	{
		FUN[main_menu_index][sub_menu_index]();
	}
}



