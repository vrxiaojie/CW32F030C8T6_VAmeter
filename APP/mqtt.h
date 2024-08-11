<<<<<<< HEAD
/**
* @file         mqtt.h
* @brief		MQTT服务相关操作的头文件
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
#ifndef MQTT_H_
#define MQTT_H_

	#include "board.h"
	#include "esp8266_cmd.h"
	#include "GUI.h"
	#include "Lcd_Driver.h"
	#include "menu.h"
	#include <stdlib.h>

	/*注意CLIENTID当中的逗号，除了第一个之外，其他的逗号前都要加两个反斜杠 \\  转义*/
<<<<<<< HEAD
	#define USERCFG "AT+MQTTUSERCFG=0,1,\"NULL\",\"username\",\"password\",0,0,\"\""  ///<设置MQTT用户名和密码
	#define CLIENTID "AT+MQTTCLIENTID=0,\"ClientID\""  ///<配置ClientID
	#define MQTTCONN "AT+MQTTCONN=0,\"MQTTHost\",Port,1"  ///<配置MQTT服务器地址MQTTHost与端口Port
	#define MQTT_TOPIC "TOPIC"  ///<配置要发布消息的TOPIC
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
