/**
* @file         mqtt.h
* @brief		MQTT服务相关操作的头文件
* @author		VRxiaojie
* @date		    2024-08-11
* @version	    v1.0
* @note 	 	哔哩哔哩	:		VR小杰		https://space.bilibili.com/11526854 <br>
* 				嘉立创开源广场:	vrxiaojie	https://oshwhub.com/vrxiaojie/	
*/

#ifndef MQTT_H_
#define MQTT_H_

	#include "board.h"
	#include "esp8266_cmd.h"
	#include "GUI.h"
	#include "Lcd_Driver.h"
	#include "menu.h"
	#include <stdlib.h>

	/*注意CLIENTID当中的逗号，除了第一个之外，其他的逗号前都要加两个反斜杠 \\  转义*/
	#define USERCFG "AT+MQTTUSERCFG=0,1,\"NULL\",\"(修改)用户名\",\"(修改)密码\",0,0,\"\""  ///<设置MQTT用户名和密码
	#define CLIENTID "AT+MQTTCLIENTID=0,\"(修改)ClientID\""  ///<配置ClientID
	#define MQTTCONN "AT+MQTTCONN=0,\"(修改)address\",(修改)端口号,1"  ///<配置MQTT服务器地址MQTTHost与端口Port
	#define MQTT_TOPIC "(修改)TOPIC"  ///<配置要发布消息的TOPIC

	void MQTT_Setup(void);
	void MQTT_Disconnect(void);
	void MQTT_Publish(char *data);

	extern uint8_t mqtt_status;

#endif
