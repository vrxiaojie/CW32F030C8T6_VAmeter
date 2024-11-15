/**
* @file         mqtt.h
* @brief		MQTT������ز�����ͷ�ļ�
* @author		VRxiaojie
* @date		    2024-08-11
* @version	    v1.0
* @note 	 	��������	:		VRС��		https://space.bilibili.com/11526854 <br>
* 				��������Դ�㳡:	vrxiaojie	https://oshwhub.com/vrxiaojie/	
*/

#ifndef MQTT_H_
#define MQTT_H_

	#include "board.h"
	#include "esp8266_cmd.h"
	#include "GUI.h"
	#include "Lcd_Driver.h"
	#include "menu.h"
	#include <stdlib.h>

	/*ע��CLIENTID���еĶ��ţ����˵�һ��֮�⣬�����Ķ���ǰ��Ҫ��������б�� \\  ת��*/
	#define USERCFG "AT+MQTTUSERCFG=0,1,\"NULL\",\"(�޸�)�û���\",\"(�޸�)����\",0,0,\"\""  ///<����MQTT�û���������
	#define CLIENTID "AT+MQTTCLIENTID=0,\"(�޸�)ClientID\""  ///<����ClientID
	#define MQTTCONN "AT+MQTTCONN=0,\"(�޸�)address\",(�޸�)�˿ں�,1"  ///<����MQTT��������ַMQTTHost��˿�Port
	#define MQTT_TOPIC "(�޸�)TOPIC"  ///<����Ҫ������Ϣ��TOPIC

	void MQTT_Setup(void);
	void MQTT_Disconnect(void);
	void MQTT_Publish(char *data);

	extern uint8_t mqtt_status;

#endif
