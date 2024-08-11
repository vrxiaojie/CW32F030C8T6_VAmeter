/**
* @file         sys.h
* @brief		与菜单中"系统设置"相关操作的头文件
* @author		VRxiaojie
* @date		    2024-08-11
* @version	    v1.0
* @note 	 	哔哩哔哩	:		VR小杰		https://space.bilibili.com/11526854 <br>
* 				嘉立创开源广场:	vrxiaojie	https://oshwhub.com/vrxiaojie/	
*/

#ifndef SYS_H_
#define SYS_H_



	#include "board.h"
	#include "Lcd_Driver.h"
	#include "esp8266_cmd.h"
	#include "key.h"
	#include "tim.h"
	#include "GUI.h"
	#include "menu.h"

	void MCU_Sleep(void);
	void System_Info(void);
	void Brightness_Setup(void);
	void Enable_or_Disable_ESP(void);
	
	extern volatile uint8_t Sleep_Status;

#endif
