/**
* @file         main.c
* @brief		主函数
* @author		VRxiaojie
* @date		    2024-08-11
* @version	    v1.0
* @note 		哔哩哔哩	:		VR小杰		https://space.bilibili.com/11526854 <br>
* 				嘉立创开源广场:	vrxiaojie	https://oshwhub.com/vrxiaojie/	
*/


#include "main.h"

/**
* @brief		程序主函数
* @param		无
* @return		无
* @note			做一些初始化以及主菜单的操作判断逻辑
*/
int32_t main(void)
{
    board_init();	//开发板初始化
    Lcd_Init();  //初始化LCD屏
    Lcd_Clear(BLACK); //屏幕背景色设为黑

    uart1_init(115200);	// 串口1波特率115200
    GPIO_User_Init(); //用户定义的GPIO初始化
    BTIM1_Init();	//基本定时器1初始化
    BTIM2_Init();	//基本定时器2初始化
	GTIM3_PWM_Init();	//通用定时器3初始化
    Key_Init();		//按键初始化
    //上电后设ESP8266为无 Wi-Fi 模式，关闭自动连接WIFI（省电）
    USART_SendString(CW_UART1,(uint8_t*)"AT+CWMODE=0,0\r\n");
    Show_Main_Menu();	//显示主菜单

    while(1)
    {	
		uint8_t btn;
		for(btn = 0;btn<5;btn++)
		{
			if(key_flag[btn]==1)
				break;
		}
		if(btn<5)
			key_flag[btn]=0;
		
		//操作符在主菜单
		if(in_sub_menu_flag == 0&& in_progress_flag ==0)
		{
			switch(btn)
		{
			//向上
			case 0:
				Restore_Main_Menu_Button(main_menu_index);
				switch(main_menu_index)
				{
					case 0:
						main_menu_index = MAIN_MENU_INDEX;
						break;
					default:
						main_menu_index --;
						break;
				}
				sub_menu_index = 0;
				Choose_Main_Function();
				Show_Sub_Menu();
				break;
			//向下
			case 1:
				Restore_Main_Menu_Button(main_menu_index);
				main_menu_index = (main_menu_index+1)%(MAIN_MENU_INDEX+1);
				sub_menu_index = 0;
				Choose_Main_Function();
				Show_Sub_Menu();
				break;
			//向右
			case 3:
				in_sub_menu_flag = 1;
				Choose_Sub_Function();
				break;
		}
		}
		//操作符在次级菜单
		else if(in_sub_menu_flag == 1 && in_progress_flag ==0)
		{
			switch(btn){
			//向上
			case 0:
				Restore_Sub_Menu_Button(sub_menu_index);
				switch(sub_menu_index)
				{
					case 0:
						sub_menu_index = sub_menu_maxidx[main_menu_index];
						break;
					default:
						sub_menu_index --;
						break;
				}
				Choose_Sub_Function();
				break;
			//向下
			case 1:
				Restore_Sub_Menu_Button(sub_menu_index);
				sub_menu_index = (sub_menu_index+1)%(sub_menu_maxidx[main_menu_index]+1);
				Choose_Sub_Function();
				break;
			//向左
			case 2:
				in_sub_menu_flag = 0;
				Restore_Sub_Menu_Button(sub_menu_index);
				break;
			case 4:
				Run_Function();
				break;
			}
		}
		
		//操作左键返回主菜单
		if(in_progress_flag == 1 && btn == 2)
		{
			Lcd_Clear(BLACK);
			Show_Main_Menu();
			in_progress_flag = 0;
		}
		
		//若ESP8266状态发生变化则执行操作
		if(ESP8266_Last_Status!=ESP8266_Status && in_progress_flag == 0)
		{
			ESP8266_Last_Status=ESP8266_Status;
			if(ESP8266_Status==1)
				ESP8266_Reset(0);
			else
			{
				FUN[2][0] = Connect_WIFI;
				FUN[3][0] = MQTT_Setup;
				sub_menu_button[2][0]="连接WIFI  ";
				sub_menu_button[3][0]="连接MQTT  ";
				mqtt_status = 0;
				WIFI_Status = 0;
			}
			Show_Main_Menu();
		}
		
		if(bat_cnt>=30)//每30s刷新一次电池电量
		{
			Show_Status_Bar();
			bat_cnt=0;
		}
    }
}
