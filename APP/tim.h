<<<<<<< HEAD
/**
* @file         tim.h
* @brief		��ʱ����ص�ͷ�ļ�
* @author		VRxiaojie
* @date		    2024-08-11
* @version	    v1.0
* @note 	 	��������	:		VRС��		https://space.bilibili.com/11526854 <br>
* 				��������Դ�㳡:	vrxiaojie	https://oshwhub.com/vrxiaojie/	
*/

=======
/*
 * ���������˺�		: 	VRС��      	https://space.bilibili.com/11526854
 * ��������Դ�㳡	: 	vrxiaojie   https://oshwhub.com/vrxiaojie/
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
