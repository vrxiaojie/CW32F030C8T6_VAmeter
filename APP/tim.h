/*
 * 哔哩哔哩账号		: 	VR小杰      	https://space.bilibili.com/11526854
 * 嘉立创开源广场	: 	vrxiaojie   https://oshwhub.com/vrxiaojie/
 *
 *   Date           Author
 * 2024-08-10     VRxiaojie
 */
#ifndef TIM_H_
#define TIM_H_

	#include "board.h"
	#include "key.h"
	#include "main.h"

	#define ESP8266_EN_PIN GPIO_PIN_14
	#define ESP8266_EN_PORT CW_GPIOB
	#define ESP8266_PIN GPIO_PIN_15
	#define ESP8266_PORT CW_GPIOB

	void BTIM1_Init(void);
	void BTIM1_IRQHandler(void);
	void BTIM2_Init(void);
	void BTIM2_IRQHandler(void);
	void GTIM3_PWM_Init(void);

	extern uint8_t bat_cnt;

#endif
