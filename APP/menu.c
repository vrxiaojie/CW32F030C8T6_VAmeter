<<<<<<< HEAD
/**
* @file         menu.c
* @brief		菜单相关函数源文件
* @author		VRxiaojie
* @date		    2024-08-11
* @version	    v1.0
* @note 	 	哔哩哔哩	:		VR小杰		https://space.bilibili.com/11526854 <br>
* 				嘉立创开源广场:	vrxiaojie	https://oshwhub.com/vrxiaojie/	
*/

=======
/*
 * 哔哩哔哩账号		: 	VR小杰      	https://space.bilibili.com/11526854
 * 嘉立创开源广场	: 	vrxiaojie   https://oshwhub.com/vrxiaojie/
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


/*存放各个运行函数的数组*/
void (*FUN[5][5])() = {
{Launch_VA_Meter,Launch_VA_Meter},//电压电流
{SHT40_Show_Temp_Humid},//温湿度计
{Connect_WIFI,Network_info},//WIFI
{MQTT_Setup},//MQTT
{System_Info,ESP8266_Reset,MCU_Sleep,Brightness_Setup,Enable_or_Disable_ESP}};//系统设置
//存放主菜单按钮名称
char* main_menu_button[] = {"电压电流","温湿度计","WIFI    ","MQTT    ","系统设置"};
//存放次级菜单按钮名称
char* sub_menu_button[5][5]={{"图形模式  ","数字模式  ","          ","          ","          "},
{"温湿度    ","          ","          ","          ","          "},
{"连接WIFI  ","网络信息  ","          ","          ","          "},
{"连接MQTT  ","          ","          ","          ","          "},
{"系统参数  ","重置ESP   ","休眠      ","亮度设置  ","启动ESP8266 "}};
uint8_t sub_menu_maxidx[5] = {SUB_MENU_0_INDEX,SUB_MENU_1_INDEX,SUB_MENU_2_INDEX,SUB_MENU_3_INDEX,SUB_MENU_4_INDEX};

<<<<<<< HEAD
/**
* @brief		显示状态栏
* @param		无
* @return		无	
*/

=======
/**************************************************************************************
	函数名称：	Show_Status_Bar

	函数功能：	显示状态栏

	入口参数：	无

	返回参数：	无
**************************************************************************************/
>>>>>>> 0dfd7a7e8c0db99ec07f30fe2edd2e84eaf20952
void Show_Status_Bar()
{
	//最左侧放电池电量
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
	
    //右数第一个放WIFI状态
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

    //右数第二个放ESP8266连接状态
    switch(ESP8266_Status)
    {
    case 0 :
        showimage_16(ESP8266_ERROR,124,0);
        break;
    case 1 :
        showimage_16(ESP8266_OK,124,0);
        break;
    }
    //右数第三个放MQTT连接状态
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
* @brief		显示主菜单
* @param		无
* @return		无	
*/

=======

/**************************************************************************************
	函数名称：	Show_Main_Menu

	函数功能：	显示主菜单

	入口参数：	无

	返回参数：	无
**************************************************************************************/
>>>>>>> 0dfd7a7e8c0db99ec07f30fe2edd2e84eaf20952
void Show_Main_Menu()
{
    //画分割线
    Gui_DrawLine(0,20,160,20,GREEN);
    Gui_DrawLine(80,20,80,128,GREEN);
    //显示状态栏
    Show_Status_Bar();
    //渲染主菜单
    for(uint8_t i = 0; i<5; i++)
    {
        uint8_t x = 0,y = 22+i*20;
        Gui_DrawFont_GBK16(x,y,WHITE,BLACK,main_menu_button[i]);
    }
    Choose_Main_Function();  //高亮选中主菜单
	Show_Sub_Menu(); //同时显示当前次级菜单
	if(in_sub_menu_flag == 1) //若在次级菜单，则高亮选中次级菜单的功能
		Choose_Sub_Function();
}

<<<<<<< HEAD
/**
* @brief		显示次级菜单
* @param		无
* @return		无	
*/

=======
/**************************************************************************************
	函数名称：	Show_Sub_Menu

	函数功能：	显示次级菜单

	入口参数：	无

	返回参数：	无
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
* @brief		次级菜单按钮被选中
* @param		无
* @return		无	
*/

=======

/**************************************************************************************
	函数名称：	Choose_Sub_Function

	函数功能：	次级菜单按钮被选中

	入口参数：	无

	返回参数：	无
**************************************************************************************/
>>>>>>> 0dfd7a7e8c0db99ec07f30fe2edd2e84eaf20952
void Choose_Sub_Function()
{
	uint8_t x = 81,y=40;
    Gui_DrawLine(x,y+sub_menu_index*20,x+75,y+sub_menu_index*20,ORANGE);
}
<<<<<<< HEAD

/**
* @brief		主菜单按钮被选中
* @param		无
* @return		无	
*/

=======
/**************************************************************************************
	函数名称：	Choose_Main_Function

	函数功能：	主菜单按钮被选中

	入口参数：	无

	返回参数：	无
**************************************************************************************/
>>>>>>> 0dfd7a7e8c0db99ec07f30fe2edd2e84eaf20952
void Choose_Main_Function()
{
    uint8_t x = 0,y=40;
	Gui_DrawLine(x,y+main_menu_index*20,x+75,y+main_menu_index*20,LIGHTBLUE);
}

<<<<<<< HEAD
/**
* @brief		次级菜单按钮恢复未被选中状态
* @param		idx 之前被选中的次级菜单索引
* @return		无	
*/

=======

/**************************************************************************************
	函数名称：	Restore_Sub_Menu_Button

	函数功能：	次级菜单按钮恢复未被选中状态

	入口参数：	无

	返回参数：	无
**************************************************************************************/
>>>>>>> 0dfd7a7e8c0db99ec07f30fe2edd2e84eaf20952
void Restore_Sub_Menu_Button(uint8_t idx)
{
	uint8_t x = 81,y = 40;
	Gui_DrawLine(x,y+idx*20,x+75,y+idx*20,BLACK);
}

<<<<<<< HEAD
/**
* @brief		主菜单按钮恢复未被选中状态
* @param		idx 之前被选中的主级菜单索引
* @return		无	
*/

=======
/**************************************************************************************
	函数名称：	Restore_Main_Menu_Button

	函数功能：	主菜单按钮恢复未被选中状态

	入口参数：	无

	返回参数：	无
**************************************************************************************/
>>>>>>> 0dfd7a7e8c0db99ec07f30fe2edd2e84eaf20952
void Restore_Main_Menu_Button(uint8_t idx)
{
	uint8_t x = 0,y = 40;
	Gui_DrawLine(x,y+idx*20,x+75,y+idx*20,BLACK);
}

<<<<<<< HEAD
/**
* @brief		执行对应的功能函数
* @param		无
* @return		无	
*/

=======
/**************************************************************************************
	函数名称：	Run_Function

	函数功能：	执行对应的功能函数

	入口参数：	无

	返回参数：	无
**************************************************************************************/
>>>>>>> 0dfd7a7e8c0db99ec07f30fe2edd2e84eaf20952
void Run_Function()
{
    in_progress_flag = 1;
	Lcd_Clear(BLACK);
	if(main_menu_index==0)  //针对电压电流表菜单单独设置，用于传参
	{
		FUN[main_menu_index][sub_menu_index](sub_menu_index);
	}
		
	else
	{
		FUN[main_menu_index][sub_menu_index]();
	}
}



