/*
 * 哔哩哔哩账号		: 	VR小杰      	https://space.bilibili.com/11526854
 * 嘉立创开源广场	: 	vrxiaojie   https://oshwhub.com/vrxiaojie/
 *
 *   Date           Author
 * 2024-08-10     VRxiaojie
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

#endif
