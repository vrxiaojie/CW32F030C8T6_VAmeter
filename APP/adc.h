/**
* @file         adc.h
* @brief		����ģ��ת����ز�����ͷ�ļ�
* @author		VRxiaojie
* @date		    2024-08-11
* @version	    v1.0
* @note 	 	��������	:		VRС��		https://space.bilibili.com/11526854 <br>
* 				��������Դ�㳡:	vrxiaojie	https://oshwhub.com/vrxiaojie/	
*/

#ifndef ADC_H_
#define ADC_H_

	#include "board.h"
	#include "main.h"
	#include "GUI.h"


	#define BAT_VOL_PIN GPIO_PIN_7   ///<��ص�����������
	#define BAT_VOL_PIN_PORT CW_GPIOA  ///<��ص����������Ŷ˿�

	void Init_VAmeter_ADC(void);
	void ADC_GET(void);
	uint16_t Battery_Volume(void);

	extern uint16_t adc_value[3];

#endif
