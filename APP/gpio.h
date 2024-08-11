/**
* @file         gpio.h
* @brief		GPIO相关操作头文件
* @author		VRxiaojie
* @date		    2024-08-11
* @version	    v1.0
* @note 	 	哔哩哔哩	:		VR小杰		https://space.bilibili.com/11526854 <br>
* 				嘉立创开源广场:	vrxiaojie	https://oshwhub.com/vrxiaojie/	
*/

#ifndef GPIO_H_
#define GPIO_H_

	#include "main.h"

	#define ESP8266_EN_PIN GPIO_PIN_14
	#define ESP8266_EN_PORT CW_GPIOB
	#define ESP8266_PIN GPIO_PIN_15
	#define ESP8266_PORT CW_GPIOB
	
	void GPIO_User_Init(void);

#endif
