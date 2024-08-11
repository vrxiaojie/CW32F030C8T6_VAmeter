/**
* @file         key.c
* @brief		按键相关操作的源文件
* @author		VRxiaojie
* @date		    2024-08-11
* @version	    v1.0
* @note 	 	哔哩哔哩	:		VR小杰		https://space.bilibili.com/11526854 <br>
* 				嘉立创开源广场:	vrxiaojie	https://oshwhub.com/vrxiaojie/	
*/

#include "key.h"

//初始化按键状态变量
uint16_t key_pin[5] = {UP_KEY_PIN,DOWN_KEY_PIN,LEFT_KEY_PIN,RIGHT_KEY_PIN,CONFIRM_KEY_PIN};
GPIO_TypeDef* key_pin_port[5] = {UP_KEY_PORT,DOWN_KEY_PORT,LEFT_KEY_PORT,RIGHT_KEY_PORT,CONFIRM_KEY_PORT};
uint8_t key_status[5]={0,0,0,0,0};
uint8_t key_flag[5]={0,0,0,0,0};

/**
* @brief		按键初始化函数
* @param		无
* @return		无	
*/

void Key_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    /*
	PA8 -> UP_KEY	
	PA9->DOWN_KEY 
	PA10->LEFT_KEY
	PA11->RIGHT_KEY
	PA12->CONFIRM_KEY	
	*/
    GPIO_InitStruct.Pins	= LEFT_KEY_PIN|RIGHT_KEY_PIN|UP_KEY_PIN | CONFIRM_KEY_PIN| DOWN_KEY_PIN;
    GPIO_InitStruct.Mode	= GPIO_MODE_INPUT_PULLUP;		//上拉输入
    GPIO_InitStruct.Speed	= GPIO_SPEED_HIGH;			//输出速度高
    GPIO_Init(CW_GPIOA, &GPIO_InitStruct);				//初始化
}

/**
* @brief		使用状态机检查按键状态
* @param		无
* @return		无
* @note			key_status为0-->未按下 <br> key_status为1-->点按 <br> key_status为15-->长按
*/

void Check_Key_Status()
{

	
	for(uint8_t i = 0;i<5;i++)
	{
		if(key_status[i]==0)
		{
			if(GPIO_ReadPin(key_pin_port[i],key_pin[i])==GPIO_Pin_RESET)
			{
				key_status[i] = 1;
			}
		}
		else if(key_status[i] == 1)
		{
			if(GPIO_ReadPin(key_pin_port[i],key_pin[i])==GPIO_Pin_RESET)  //识别到短按
			{
				key_flag[i] = 1;
				key_status[i] = 2;
			}
			else
			{
				key_status[i] = 0;
			}
		}
		else if(key_status[i] >= 2 && key_status[i]<15)
		{
			if(GPIO_ReadPin(key_pin_port[i],key_pin[i])==GPIO_Pin_RESET)  //从短按到长按的中间态,按键至少按下了130ms
			{
				key_status[i]++;
			}
			else
			{
				key_status[i]--;
			}
		}
		else if(key_status[i] == 15)
		{
			if(GPIO_ReadPin(key_pin_port[i],key_pin[i])==GPIO_Pin_RESET)  //识别到长按
			{
				key_flag[i] = 2;
			}
			else
			{
				key_status[i]--;
			}
		}		
		else
		{
			if(GPIO_ReadPin(key_pin_port[i],key_pin[i])==GPIO_Pin_SET)
			{
				key_flag[i]=0;
			}
		}
	}
}
