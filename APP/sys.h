<<<<<<< HEAD
/**
* @file         sys.h
* @brief		��˵���"ϵͳ����"��ز�����ͷ�ļ�
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
#ifndef SYS_H_
#define SYS_H_



	#include "board.h"
	#include "Lcd_Driver.h"
	#include "esp8266_cmd.h"
	#include "key.h"
	#include "tim.h"
	#include "GUI.h"
	#include "menu.h"

	void MCU_Sleep(void);
	void System_Info(void);
	void Brightness_Setup(void);
	void Enable_or_Disable_ESP(void);
<<<<<<< HEAD
	
	extern volatile uint8_t Sleep_Status;
=======
>>>>>>> 0dfd7a7e8c0db99ec07f30fe2edd2e84eaf20952

#endif
