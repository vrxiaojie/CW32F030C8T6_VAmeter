/*
 * ���������˺�		: 	VRС��      	https://space.bilibili.com/11526854
 * ��������Դ�㳡	: 	vrxiaojie   https://oshwhub.com/vrxiaojie/
 *
 *   Date           Author
 * 2024-08-10     VRxiaojie
 */

#include "key.h"
/**************************************************************************************
	�������ƣ�	Key_Init

	�������ܣ�	������ʼ������

	��ڲ�����	��

	���ز�����	��
**************************************************************************************/
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
    GPIO_InitStruct.Mode	= GPIO_MODE_INPUT_PULLUP;		//��������
    GPIO_InitStruct.Speed	= GPIO_SPEED_HIGH;			//����ٶȸ�
    GPIO_Init(CW_GPIOA, &GPIO_InitStruct);				//��ʼ��
}

/**************************************************************************************
	�������ƣ�	Check_Key_Status

	�������ܣ�	ʹ��״̬����鰴��״̬

	��ڲ�����	��

	���ز�����	��
**************************************************************************************/
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
			if(GPIO_ReadPin(key_pin_port[i],key_pin[i])==GPIO_Pin_RESET)  //ʶ�𵽶̰�
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
			if(GPIO_ReadPin(key_pin_port[i],key_pin[i])==GPIO_Pin_RESET)  //�Ӷ̰����������м�̬,�������ٰ�����130ms
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
			if(GPIO_ReadPin(key_pin_port[i],key_pin[i])==GPIO_Pin_RESET)  //ʶ�𵽳���
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
