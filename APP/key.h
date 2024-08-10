/*
 * 哔哩哔哩账号		: 	VR小杰      	https://space.bilibili.com/11526854
 * 嘉立创开源广场	: 	vrxiaojie   https://oshwhub.com/vrxiaojie/
 *
 *   Date           Author
 * 2024-08-10     VRxiaojie
 */
#ifndef KEY_H_
#define KEY_H_

	#include "board.h"
	#include "main.h"

	void Check_Key_Status(void);
	void Key_Init(void);

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

#endif


