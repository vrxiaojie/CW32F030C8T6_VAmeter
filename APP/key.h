/**
* @file         key.h
* @brief		按键相关操作的头文件
* @author		VRxiaojie
* @date		    2024-08-11
* @version	    v1.0
* @note 	 	哔哩哔哩	:		VR小杰		https://space.bilibili.com/11526854 <br>
* 				嘉立创开源广场:	vrxiaojie	https://oshwhub.com/vrxiaojie/	
*/

#ifndef KEY_H_
#define KEY_H_

	#include "board.h"
	#include "main.h"

	#define UP_KEY_PIN GPIO_PIN_12
	#define DOWN_KEY_PIN GPIO_PIN_8
	#define LEFT_KEY_PIN GPIO_PIN_9
	#define RIGHT_KEY_PIN GPIO_PIN_11
	#define CONFIRM_KEY_PIN GPIO_PIN_10


	#define UP_KEY_PORT CW_GPIOA
	#define DOWN_KEY_PORT CW_GPIOA
	#define LEFT_KEY_PORT CW_GPIOA
	#define RIGHT_KEY_PORT CW_GPIOA
	#define CONFIRM_KEY_PORT CW_GPIOA
	
	void Check_Key_Status(void);
	void Key_Init(void);

	extern uint16_t key_pin[5];
	extern GPIO_TypeDef* key_pin_port[5];
	extern uint8_t key_status[5];
	extern uint8_t key_flag[5];

#endif


