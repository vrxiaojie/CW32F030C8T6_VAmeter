/*
 * 哔哩哔哩账号		: 	VR小杰      	https://space.bilibili.com/11526854
 * 嘉立创开源广场	: 	vrxiaojie   https://oshwhub.com/vrxiaojie/
 *
 *   Date           Author
 * 2024-08-10     VRxiaojie
 */

#ifndef MAIN_H_
#define MAIN_H_

	#include "board.h"
	#include "stdio.h"
	#include <stdlib.h>
	#include "bsp_uart.h"
	#include "Lcd_Driver.h"
	#include "esp8266_cmd.h"
	#include "menu.h"
	#include "key.h"
	#include "tim.h"
	#include "mqtt.h"
	#include "adc.h"
	#include "sht40.h"


	//设置WIFI名称和密码
	#define WIFI_SSID ""
	#define WIFI_PASSWORD ""

	extern uint8_t ESP8266_Status;
	extern volatile uint8_t Sleep_Status;
	extern uint8_t in_sub_menu_flag;
	extern uint8_t ESP8266_Last_Status;
	 
	extern uint16_t key_pin[5];
	extern GPIO_TypeDef* key_pin_port[5];
	extern uint8_t key_status[5];
	extern uint8_t key_flag[5];

	void MCU_Sleep(void);

#endif
