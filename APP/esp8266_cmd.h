<<<<<<< HEAD
/**
* @file         esp8266_cmd.h
* @brief		ESP8266相关操作头文件
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
#ifndef ESP8266_CMD_H_
#define ESP8266_CMD_H_

	#include "board.h"
	#include "str.h"

	uint8_t ESP8266_SendCmd(uint8_t *cmd, uint8_t *res);
	void USART_SendString(UART_TypeDef* USARTx, uint8_t *DataString);
	void ESP8266_SendData(uint8_t *data, uint16_t len);
	void ESP8266_Clear(void);
	void Network_info(void);
	void Connect_WIFI(void);
	void Disconnect_WIFI(void);
	void ESP8266_Reset(uint8_t echo);
<<<<<<< HEAD
=======
	char* strcat_new(char* s1,char* s2);
>>>>>>> 0dfd7a7e8c0db99ec07f30fe2edd2e84eaf20952

	extern uint8_t esp8266_buf[1000];
	extern uint16_t esp8266_cnt, esp8266_cntPre;
	extern uint8_t WIFI_Status;
<<<<<<< HEAD
	extern uint8_t ESP8266_Status;
	extern uint8_t ESP8266_Last_Status;
=======
	extern char buffer[300];
>>>>>>> 0dfd7a7e8c0db99ec07f30fe2edd2e84eaf20952

#endif
