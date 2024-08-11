/**
* @file         gpio.c
* @brief		GPIO��ز���Դ�ļ�
* @author		VRxiaojie
* @date		    2024-08-11
* @version	    v1.0
* @note 	 	��������	:		VRС��		https://space.bilibili.com/11526854 <br>
* 				��������Դ�㳡:	vrxiaojie	https://oshwhub.com/vrxiaojie/	
*/

#include "gpio.h"

/**
* @brief		GPIOA�жϴ�����
* @param		��
* @return		��
*/

void GPIOA_IRQHandler(void)
{
    if(CW_GPIOA->ISR_f.PIN10)

    {
        GPIOA_INTFLAG_CLR(bv10);//���CW_GPIO�жϱ�־
    }

}

/**
* @brief		��ʼ���û������GPIO�˿�
* @param		��
* @return		��
*/
void GPIO_User_Init()
{
    GPIO_InitTypeDef	GPIO_InitStruct; // GPIO��ʼ���ṹ��

    __RCC_GPIOA_CLK_ENABLE();// ʹ��GPIOʱ��
    __RCC_GPIOB_CLK_ENABLE();
    __RCC_GPIOC_CLK_ENABLE();

    GPIO_InitStruct.Pins = ESP8266_EN_PIN;			// PB14��ΪESP8266����ͣ����
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;		//�������
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;		// ����ٶȸ�
    GPIO_Init(ESP8266_EN_PORT, &GPIO_InitStruct);			// ��ʼ��

    GPIO_WritePin(ESP8266_EN_PORT,ESP8266_EN_PIN,GPIO_Pin_RESET); //Ĭ�Ϲر�ESP8266

    GPIO_InitStruct.Pins = ESP8266_PIN;				// PB15��Ϊ���ESP8266״̬������
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;			//��������
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;		// ����ٶȸ�
    GPIO_Init(ESP8266_PORT, &GPIO_InitStruct);		// ��ʼ��
	
	GPIO_InitStruct.Pins = GPIO_PIN_13;				// PB13��ΪLDO����ͣ����
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;		//�������
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;		// ����ٶȸ�
    GPIO_Init(CW_GPIOB, &GPIO_InitStruct);			// ��ʼ��
	GPIO_WritePin(CW_GPIOB,GPIO_PIN_13,GPIO_Pin_SET);  //Ĭ�Ͽ���LDO
	
	GPIO_InitStruct.Pins = SCL_PIN|SDA_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	GPIO_Init(CW_GPIOA, &GPIO_InitStruct);
}

