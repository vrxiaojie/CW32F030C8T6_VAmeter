/**
* @file         va_meter.h
* @brief		��ѹ��������ز�����ͷ�ļ�
* @author		VRxiaojie
* @date		    2024-08-11
* @version	    v1.0
* @note 	 	��������	:		VRС��		https://space.bilibili.com/11526854 <br>
* 				��������Դ�㳡:	vrxiaojie	https://oshwhub.com/vrxiaojie/	
*/

#ifndef VA_METER_H_
#define VA_METER_H_

	#include "board.h"
	#include "GUI.h"
	#include "adc.h"
	#include "Lcd_Driver.h"

	void Launch_VA_Meter(uint8_t idx);
	void VAmeter_Mqtt_Send_Data(double volt,double curr);

#endif
