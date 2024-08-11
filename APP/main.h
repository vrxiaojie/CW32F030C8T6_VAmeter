<<<<<<< HEAD
/**
* @file         main.h
* @brief		主函数的头文件
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

#ifndef MAIN_H_
#define MAIN_H_

	#include "board.h"
<<<<<<< HEAD
	#include <stdio.h>
=======
	#include "stdio.h"
>>>>>>> 0dfd7a7e8c0db99ec07f30fe2edd2e84eaf20952
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
<<<<<<< HEAD
	#include "gpio.h"

	#define WIFI_SSID ""  ///<WIFI名
	#define WIFI_PASSWORD "" ///<WIFI密码
=======


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
>>>>>>> 0dfd7a7e8c0db99ec07f30fe2edd2e84eaf20952

#endif
