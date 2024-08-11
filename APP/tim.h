<<<<<<< HEAD
/**
* @file         tim.h
* @brief		定时器相关的头文件
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
#ifndef TIM_H_
#define TIM_H_

	#include "board.h"
	#include "key.h"
	#include "main.h"

<<<<<<< HEAD
=======
	#define ESP8266_EN_PIN GPIO_PIN_14
	#define ESP8266_EN_PORT CW_GPIOB
	#define ESP8266_PIN GPIO_PIN_15
	#define ESP8266_PORT CW_GPIOB

>>>>>>> 0dfd7a7e8c0db99ec07f30fe2edd2e84eaf20952
	void BTIM1_Init(void);
	void BTIM1_IRQHandler(void);
	void BTIM2_Init(void);
	void BTIM2_IRQHandler(void);
	void GTIM3_PWM_Init(void);

	extern uint8_t bat_cnt;

#endif
