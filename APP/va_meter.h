<<<<<<< HEAD
/**
* @file         va_meter.h
* @brief		��ѹ��������ز�����ͷ�ļ�
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
#ifndef VA_METER_H_
#define VA_METER_H_

	#include "board.h"
	#include "GUI.h"
	#include "adc.h"
	#include "Lcd_Driver.h"

	void Launch_VA_Meter(uint8_t idx);
	void VAmeter_Mqtt_Send_Data(double volt,double curr);

#endif
