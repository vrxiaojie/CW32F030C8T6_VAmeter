/*
 * ���������˺�		: 	VRС��      	https://space.bilibili.com/11526854
 * ��������Դ�㳡	: 	vrxiaojie   https://oshwhub.com/vrxiaojie/
 *
 *   Date           Author
 * 2024-08-10     VRxiaojie
 */
#include "tim.h"
#include "esp8266_cmd.h"
#include "menu.h"
#include "Lcd_Driver.h"

uint8_t bat_cnt=0;

/******************************************************************
 * �� �� �� �ƣ�BTIM1_Init
 * �� �� ˵ ������ʱ��BTIM1��ʼ������  10ms �ж�һ��
 * �� �� �� �Σ���
 * �� �� �� �أ���
******************************************************************/
void BTIM1_Init(void)
{
    // ʹ��BTIM1��ʱ��
    __RCC_BTIM_CLK_ENABLE();

    // ��ֹ�жϣ��԰�ȫ������NVIC
    __disable_irq();

    // ����BTIM1�жϣ���������NVIC
    NVIC_EnableIRQ(BTIM1_IRQn);

    // �����жϣ��ָ��ж�״̬
    __enable_irq();

    // ���岢��ʼ����ʱ��ʱ���׼�ṹ��
    BTIM_TimeBaseInitTypeDef BTIM_TimeBaseInitStruct;

    // ���ö�ʱ��ģʽ�����ں�Ԥ��Ƶ��
    BTIM_TimeBaseInitStruct.BTIM_Mode = BTIM_Mode_TIMER; // ����Ϊ��ʱ��ģʽ
    BTIM_TimeBaseInitStruct.BTIM_Period = 625-1;   // �������� 16us*625=10ms����ÿ10ms�ж�һ��
    BTIM_TimeBaseInitStruct.BTIM_Prescaler = BTIM_PRS_DIV1024; // ʱ��64MHz��Ԥ��Ƶ������Ϊ1024����ƵΪ62.5kHz��������Ϊ16us
    // ʹ���������ó�ʼ����ʱ��BTIM1
    BTIM_TimeBaseInit(CW_BTIM1, &BTIM_TimeBaseInitStruct);

    // ʹ��BTIM1������ж�
    BTIM_ITConfig(CW_BTIM1, BTIM_IT_OV, ENABLE);
    // ������ʱ��BTIM1
    BTIM_Cmd(CW_BTIM1, ENABLE);
}


/******************************************************************
 * �� �� �� �ƣ�BTIM1_IRQHandler
 * �� �� ˵ ������ʱ��BTIM1�жϷ��������ô����жϰ���״̬
 * �� �� �� �Σ���
 * �� �� �� �أ���
******************************************************************/
void BTIM1_IRQHandler(void)
{
    // �ж��Ƿ�Ϊ����ж�
    if (BTIM_GetITStatus(CW_BTIM1, BTIM_IT_OV))
    {
		//��鰴��״̬�����Ž���key.c
		Check_Key_Status();
		// �������жϱ�־
        BTIM_ClearITPendingBit(CW_BTIM1, BTIM_IT_OV);
    }
}

/******************************************************************
 * �� �� �� �ƣ�BTIM2_Init
 * �� �� ˵ ������ʱ��BTIM1��ʼ������  1s �ж�һ��
 * �� �� �� �Σ���
 * �� �� �� �أ���
******************************************************************/
void BTIM2_Init(void)
{
    // ʹ��BTIM2��ʱ��
    __RCC_BTIM_CLK_ENABLE();

    // ��ֹ�жϣ��԰�ȫ������NVIC
    __disable_irq();

    // ����BTIM2�жϣ���������NVIC
    NVIC_EnableIRQ(BTIM2_IRQn);

    // �����жϣ��ָ��ж�״̬
    __enable_irq();

    // ���岢��ʼ����ʱ��ʱ���׼�ṹ��
    BTIM_TimeBaseInitTypeDef BTIM_TimeBaseInitStruct;

    // ���ö�ʱ��ģʽ�����ں�Ԥ��Ƶ��
    BTIM_TimeBaseInitStruct.BTIM_Mode = BTIM_Mode_TIMER; // ����Ϊ��ʱ��ģʽ
    BTIM_TimeBaseInitStruct.BTIM_Period = 15625-1;   // �������� 64us*15625=1s����ÿ1s�ж�һ��
    BTIM_TimeBaseInitStruct.BTIM_Prescaler = BTIM_PRS_DIV4096; // ʱ��64MHz��Ԥ��Ƶ������Ϊ4096����ƵΪ15.625kHz��������Ϊ64us
    // ʹ���������ó�ʼ����ʱ��BTIM2
    BTIM_TimeBaseInit(CW_BTIM2, &BTIM_TimeBaseInitStruct);

    // ʹ��BTIM2������ж�
    BTIM_ITConfig(CW_BTIM2, BTIM_IT_OV, ENABLE);
    // ������ʱ��BTIM2
    BTIM_Cmd(CW_BTIM2, ENABLE);
}


/******************************************************************
 * �� �� �� �ƣ�BTIM2_Init
 * �� �� ˵ ������ʱ��BTIM2�жϷ����������ڸ���״̬��
 * �� �� �� �Σ���
 * �� �� �� �أ���
******************************************************************/
void BTIM2_IRQHandler(void)
{
    // �ж��Ƿ�Ϊ����ж�
    if (BTIM_GetITStatus(CW_BTIM2, BTIM_IT_OV))
    {
        //��ESP8266ʹ�����Ÿߵ�ƽ����ESP8266�������Ҳ�Ǹߵ�ƽʱ��˵��ESP8266������
        if((GPIO_ReadPin(ESP8266_PORT,ESP8266_PIN)==GPIO_Pin_SET)&&(GPIO_ReadPin(ESP8266_EN_PORT,ESP8266_EN_PIN)==GPIO_Pin_SET))
        {
            ESP8266_Status = 1;
        }
        else
        {
            ESP8266_Status = 0;
			WIFI_Status = 0;
        }
		if(in_progress_flag==0)
		{
			bat_cnt++;
		}
        // �������жϱ�־
        BTIM_ClearITPendingBit(CW_BTIM2, BTIM_IT_OV);
    }
}


/******************************************************************
 * �� �� �� GTIM3_PWM_Init
 * �� �� ˵ ������ʱ��GTIM3��ʼ��������������ĻPWM����
 * �� �� �� �Σ���
 * �� �� �� �أ���
******************************************************************/
void GTIM3_PWM_Init(void)
{
	PC15_AFx_GTIM3CH2();						// ʹ�ø��ù��ܣ�����Ϊͨ�ö�ʱ��3ͨ��2
	/*********** GTIM3���� ***********/
	GTIM_InitTypeDef GTIM_InitStruct;  // ͨ�ö�ʱ����ʼ���ṹ��
	
    __RCC_GTIM3_CLK_ENABLE();		   // ʹ��ͨ�ö�ʱ��1ʱ��
	
    GTIM_InitStruct.Mode = GTIM_MODE_TIME;				 // ��ʱ��ģʽ
    GTIM_InitStruct.OneShotMode = GTIM_COUNT_CONTINUE;	 // ��������ģʽ
    GTIM_InitStruct.Prescaler = GTIM_PRESCALER_DIV512;    // DCLK = PCLK / 128 = 64MHz/512 = 125KHz
    GTIM_InitStruct.ReloadValue = 100;	 			 	 // ��װ��ֵ����  PWM:1.25KHz
    GTIM_InitStruct.ToggleOutState = DISABLE;			 // �����ת����
    GTIM_TimeBaseInit(CW_GTIM3, &GTIM_InitStruct);		 // ��ʼ��
	
    GTIM_OCInit(CW_GTIM3, GTIM_CHANNEL2, GTIM_OC_OUTPUT_PWM_HIGH); // ��������Ƚ�ͨ��3ΪPWMģʽ
	GTIM_SetCompare2(CW_GTIM3, lcd_brightness);  //���ó�ʼռ�ձ�Ϊlcd_brightness/100 = 50/100 = 50%
    GTIM_Cmd(CW_GTIM3, ENABLE);     // ʹ�ܶ�ʱ��
}

