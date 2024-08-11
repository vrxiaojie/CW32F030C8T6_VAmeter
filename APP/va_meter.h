<<<<<<< HEAD
/**
* @file         va_meter.h
* @brief		电压电流表相关操作的头文件
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
#ifndef VA_METER_H_
#define VA_METER_H_

	#include "board.h"
	#include "GUI.h"
	#include "adc.h"
	#include "Lcd_Driver.h"

	void Launch_VA_Meter(uint8_t idx);
	void VAmeter_Mqtt_Send_Data(double volt,double curr);

#endif
