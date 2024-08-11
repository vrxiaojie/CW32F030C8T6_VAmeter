/**
* @file         esp8266_cmd.h
* @brief		ESP8266��ز���ͷ�ļ�
* @author		VRxiaojie
* @date		    2024-08-11
* @version	    v1.0
* @note 	 	��������	:		VRС��		https://space.bilibili.com/11526854 <br>
* 				��������Դ�㳡:	vrxiaojie	https://oshwhub.com/vrxiaojie/	
*/

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

	extern uint8_t esp8266_buf[1000];
	extern uint16_t esp8266_cnt, esp8266_cntPre;
	extern uint8_t WIFI_Status;
	extern uint8_t ESP8266_Status;
	extern uint8_t ESP8266_Last_Status;

#endif
