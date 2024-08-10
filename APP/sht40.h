/*
 * ���������˺�		: 	VRС��      	https://space.bilibili.com/11526854
 * ��������Դ�㳡	: 	vrxiaojie   https://oshwhub.com/vrxiaojie/
 *
 *   Date           Author
 * 2024-08-10     VRxiaojie
 */
#ifndef SHT40_H_
#define SHT40_H_

	#include "board.h"
	#include "key.h"
	#include "GUI.h"
	#include "Lcd_Driver.h"
	#include <math.h>

	//����SCL��SDA������
	#define SDA_PIN 	GPIO_PIN_5
	#define SDA_PORT 	CW_GPIOA
	#define SCL_PIN 	GPIO_PIN_6
	#define SCL_PORT 	CW_GPIOA

	#define READ_SDA() GPIO_ReadPin(SDA_PORT, SDA_PIN)
	#define SDA_HIGH() GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_Pin_SET)
	#define SDA_LOW()  GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_Pin_RESET)
	#define SCL_HIGH() GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_Pin_SET)
	#define SCL_LOW()  GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_Pin_RESET)

	// SHT40����
	#define SHT40_ADDRESS 0x44 // SHT40��I2C��ַ
	#define SHT40_COMMAND_MEASURE_HIGH_PRECISION 0xFD  //Ҫ���͵����� 0XFD

	void SHT40_Show_Temp_Humid(void);

#endif
