/*
 * 哔哩哔哩账号		: 	VR小杰      	https://space.bilibili.com/11526854
 * 嘉立创开源广场	: 	vrxiaojie   https://oshwhub.com/vrxiaojie/
 *
 *   Date           Author
 * 2024-08-10     VRxiaojie
 */
#ifndef ADC_H_
#define ADC_H_

	#include "board.h"
	#include "main.h"
	#include "GUI.h"


	#define BAT_VOL_PIN GPIO_PIN_7
	#define BAT_VOL_PIN_PORT CW_GPIOA

	void Init_VAmeter_ADC(void);
	void ADC_GET(void);
	uint16_t Battery_Volume(void);

	extern uint16_t adc_value[3];

#endif
