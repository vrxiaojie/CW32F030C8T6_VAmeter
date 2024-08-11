<<<<<<< HEAD
/**
* @file         mqtt.h
* @brief		MQTT������ز�����ͷ�ļ�
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
#ifndef MQTT_H_
#define MQTT_H_

	#include "board.h"
	#include "esp8266_cmd.h"
	#include "GUI.h"
	#include "Lcd_Driver.h"
	#include "menu.h"
	#include <stdlib.h>

	/*ע��CLIENTID���еĶ��ţ����˵�һ��֮�⣬�����Ķ���ǰ��Ҫ��������б�� \\  ת��*/
<<<<<<< HEAD
	#define USERCFG "AT+MQTTUSERCFG=0,1,\"NULL\",\"username\",\"password\",0,0,\"\""  ///<����MQTT�û���������
	#define CLIENTID "AT+MQTTCLIENTID=0,\"ClientID\""  ///<����ClientID
	#define MQTTCONN "AT+MQTTCONN=0,\"MQTTHost\",Port,1"  ///<����MQTT��������ַMQTTHost��˿�Port
	#define MQTT_TOPIC "TOPIC"  ///<����Ҫ������Ϣ��TOPIC
=======
	#define USERCFG "AT+MQTTUSERCFG=0,1,\"NULL\",\"esp8266\",\"123456\",0,0,\"\""
	#define CLIENTID "AT+MQTTCLIENTID=0,\"esp8266_VAmeter\""
	#define MQTTCONN "AT+MQTTCONN=0,\"192.168.50.111\",1883,1"
	#define MQTT_TOPIC "/VA_Meter/value"
>>>>>>> 0dfd7a7e8c0db99ec07f30fe2edd2e84eaf20952

	void MQTT_Setup(void);
	void MQTT_Disconnect(void);
	void MQTT_Publish(char *data);

	extern uint8_t mqtt_status;

#endif
