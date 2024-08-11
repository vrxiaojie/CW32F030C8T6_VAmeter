/**
* @file         tim.h
* @brief		��ʱ����ص�ͷ�ļ�
* @author		VRxiaojie
* @date		    2024-08-11
* @version	    v1.0
* @note 	 	��������	:		VRС��		https://space.bilibili.com/11526854 <br>
* 				��������Դ�㳡:	vrxiaojie	https://oshwhub.com/vrxiaojie/	
*/

#ifndef TIM_H_
#define TIM_H_

	#include "board.h"
	#include "key.h"
	#include "main.h"

	void BTIM1_Init(void);
	void BTIM1_IRQHandler(void);
	void BTIM2_Init(void);
	void BTIM2_IRQHandler(void);
	void GTIM3_PWM_Init(void);

	extern uint8_t bat_cnt;

#endif
