/*
 * ���������˺�		: 	VRС��      	https://space.bilibili.com/11526854
 * ��������Դ�㳡	: 	vrxiaojie   https://oshwhub.com/vrxiaojie/
 *
 *   Date           Author
 * 2024-08-10     VRxiaojie
 */
#include "sys.h"


/**************************************************************************************
	�������ƣ�	MCU_Sleep

	�������ܣ�	��ϵͳ��������

	��ڲ�����	��

	���ز�����	��
**************************************************************************************/
void MCU_Sleep() {

    //��CONFIRM������ʱ��Ϊ���Ѱ���
    GPIO_InitTypeDef GPIO_InitStructure;
    __RCC_GPIOA_CLK_ENABLE();
    GPIO_InitStructure.Pins = CONFIRM_KEY_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT_PULLUP;
    GPIO_InitStructure.IT = GPIO_IT_FALLING;
    GPIO_Init(CONFIRM_KEY_PORT, &GPIO_InitStructure);
    GPIOA_INTFLAG_CLR(bv10);//���CW_GPIO�жϱ�־
    NVIC_EnableIRQ(GPIOA_IRQn);
	
	GPIO_WritePin(CW_GPIOB,GPIO_PIN_13,GPIO_Pin_RESET); //�ص�LDO

    // ʧ��GPIO B��C��ʱ�ӣ�����ʧ��GPIOA��ʱ�ӣ������޷����ѣ�
    __RCC_GPIOB_CLK_DISABLE();
    __RCC_GPIOC_CLK_DISABLE();

    PWR_InitTypeDef PWR_InitStructure;
    RCC_WAKEUPCLK_Config(RCC_SYSCTRL_WAKEUPCLKEN);
    PWR_InitStructure.PWR_Sevonpend = PWR_Sevonpend_Disable;
    PWR_InitStructure.PWR_SleepDeep = PWR_SleepDeep_Enable;
    PWR_InitStructure.PWR_SleepOnExit = PWR_SleepOnExit_Disable;
    PWR_Config(&PWR_InitStructure);
    PWR_GotoLpmMode();

    //���Ѻ�
    board_init();
    // ʹ��GPIOʱ��
    __RCC_GPIOB_CLK_ENABLE();
    __RCC_GPIOC_CLK_ENABLE();
	
	GPIO_WritePin(CW_GPIOB,GPIO_PIN_13,GPIO_Pin_SET); //����LDO

    Key_Init();  //��������³�ʼ������ ��CONFIRM�������ָܻ�����
    in_progress_flag = 0;
    in_sub_menu_flag = 0;
    Show_Main_Menu();
}

/**************************************************************************************
	�������ƣ�	System_Info

	�������ܣ�	��ʾϵͳ��Ϣ

	��ڲ�����	��

	���ز�����	��
**************************************************************************************/
void System_Info()
{
    Show_Logo(30,0);
    Gui_DrawFont_GBK16(0,40,ORANGE,BLACK,"Ver 1.0");
    Gui_DrawFont_GBK16(0,56,ORANGE,BLACK,"Author:VRxiaojie");
    Gui_DrawFont_GBK16(0,72,GREEN,BLACK,"CPU Freq:64MHz");
    Gui_DrawFont_GBK16(0,88,GREEN,BLACK,"RAM:8KB");
    Gui_DrawFont_GBK16(0,104,GREEN,BLACK,"ROM:64KB");
}


/**************************************************************************************
	�������ƣ�	Brightness_Setup

	�������ܣ�	������Ļ����

	��ڲ�����	��

	���ز�����	��
**************************************************************************************/
void Brightness_Setup()
{
    Gui_DrawFont_GBK16(48,0,LIGHTBLUE,BLACK,"��������");
	Gui_DrawFont_GBK16(8,20,LIGHTGREEN,BLACK,"Use UP or DOWN Key");
    Gui_DrawLine(20,60,141,60,WHITE);
    Gui_DrawLine(20,60,20,72,WHITE);
    Gui_DrawLine(20,72,141,72,WHITE);
    Gui_DrawLine(141,60,141,72,WHITE);
    uint8_t i,j,k;
    for(i=0; i<lcd_brightness/10; i++)
    {
        for(j=0; j<12; j++)
            for(k=0; k<10; k++)
                Gui_DrawPoint(21+(12*i)+j,k+61,DARKBLUE);
    }
    for(i=lcd_brightness/10; i<10; i++)
    {
        for(j=0; j<12; j++)
            for(k=0; k<10; k++)
                Gui_DrawPoint(21+(12*i)+j,k+61,BLACK);
    }
	key_flag[0]=0;
	key_flag[1]=0;
    while(key_flag[2]==0) //���ؼ�һֱδ������
    {
        if(key_flag[0]==1||key_flag[0]==2)
        {
            if(lcd_brightness+10<=100)
            {
                lcd_brightness+=10;
                Set_Lcd_Brightness(lcd_brightness);
                key_flag[0]=0;
                for(i=0; i<lcd_brightness/10; i++)
                {
                    for(j=0; j<12; j++)
                        for(k=0; k<10; k++)
                            Gui_DrawPoint(21+(12*i)+j,k+61,DARKBLUE);
                }
                for(i=lcd_brightness/10; i<10; i++)
                {
                    for(j=0; j<12; j++)
                        for(k=0; k<10; k++)
                            Gui_DrawPoint(21+(12*i)+j,k+61,BLACK);
                }
            }

        }
        if(key_flag[1]==1||key_flag[1]==2)
        {
            if(lcd_brightness-10>=10)
            {
                lcd_brightness-=10;
                Set_Lcd_Brightness(lcd_brightness);
                key_flag[1]=0;
                for(i=0; i<lcd_brightness/10; i++)
                {
                    for(j=0; j<12; j++)
                        for(k=0; k<10; k++)
                            Gui_DrawPoint(21+(12*i)+j,k+61,DARKBLUE);
                }
                for(i=lcd_brightness/10; i<10; i++)
                {
                    for(j=0; j<12; j++)
                        for(k=0; k<10; k++)
                            Gui_DrawPoint(21+(12*i)+j,k+61,BLACK);
                }
            }

        }
    }

}

/**************************************************************************************
	�������ƣ�	Enable_or_Disable_ESP

	�������ܣ�	������ֹͣESP8266

	��ڲ�����	��

	���ز�����	��
**************************************************************************************/
void Enable_or_Disable_ESP()
{
	if(ESP8266_Status == 0)
	{
		GPIO_WritePin(ESP8266_EN_PORT,ESP8266_EN_PIN,GPIO_Pin_SET);
		sub_menu_button[4][4]="ֹͣ8266  ";
	}
	else
	{
		GPIO_WritePin(ESP8266_EN_PORT,ESP8266_EN_PIN,GPIO_Pin_RESET);
		sub_menu_button[4][4]="����8266  ";
	}
	in_progress_flag = 0;
	Show_Main_Menu();
}
