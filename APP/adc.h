<<<<<<< HEAD
/**
* @file         adc.h
* @brief		����ģ��ת����ز�����ͷ�ļ�
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
#ifndef ADC_H_
#define ADC_H_

	#include "board.h"
	#include "main.h"
	#include "GUI.h"


<<<<<<< HEAD
	#define BAT_VOL_PIN GPIO_PIN_7   ///<��ص�����������
	#define BAT_VOL_PIN_PORT CW_GPIOA  ///<��ص����������Ŷ˿�
=======
	#define BAT_VOL_PIN GPIO_PIN_7
	#define BAT_VOL_PIN_PORT CW_GPIOA
>>>>>>> 0dfd7a7e8c0db99ec07f30fe2edd2e84eaf20952

	void Init_VAmeter_ADC(void);
	void ADC_GET(void);
	uint16_t Battery_Volume(void);

	extern uint16_t adc_value[3];

#endif
