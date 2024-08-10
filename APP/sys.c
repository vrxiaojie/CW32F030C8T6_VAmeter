/*
 * 哔哩哔哩账号		: 	VR小杰      	https://space.bilibili.com/11526854
 * 嘉立创开源广场	: 	vrxiaojie   https://oshwhub.com/vrxiaojie/
 *
 *   Date           Author
 * 2024-08-10     VRxiaojie
 */
#include "sys.h"


/**************************************************************************************
	函数名称：	MCU_Sleep

	函数功能：	让系统进入休眠

	入口参数：	无

	返回参数：	无
**************************************************************************************/
void MCU_Sleep() {

    //将CONFIRM引脚临时作为唤醒按键
    GPIO_InitTypeDef GPIO_InitStructure;
    __RCC_GPIOA_CLK_ENABLE();
    GPIO_InitStructure.Pins = CONFIRM_KEY_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT_PULLUP;
    GPIO_InitStructure.IT = GPIO_IT_FALLING;
    GPIO_Init(CONFIRM_KEY_PORT, &GPIO_InitStructure);
    GPIOA_INTFLAG_CLR(bv10);//清除CW_GPIO中断标志
    NVIC_EnableIRQ(GPIOA_IRQn);
	
	GPIO_WritePin(CW_GPIOB,GPIO_PIN_13,GPIO_Pin_RESET); //关掉LDO

    // 失能GPIO B和C的时钟（不能失能GPIOA的时钟，否则无法唤醒）
    __RCC_GPIOB_CLK_DISABLE();
    __RCC_GPIOC_CLK_DISABLE();

    PWR_InitTypeDef PWR_InitStructure;
    RCC_WAKEUPCLK_Config(RCC_SYSCTRL_WAKEUPCLKEN);
    PWR_InitStructure.PWR_Sevonpend = PWR_Sevonpend_Disable;
    PWR_InitStructure.PWR_SleepDeep = PWR_SleepDeep_Enable;
    PWR_InitStructure.PWR_SleepOnExit = PWR_SleepOnExit_Disable;
    PWR_Config(&PWR_InitStructure);
    PWR_GotoLpmMode();

    //唤醒后
    board_init();
    // 使能GPIO时钟
    __RCC_GPIOB_CLK_ENABLE();
    __RCC_GPIOC_CLK_ENABLE();
	
	GPIO_WritePin(CW_GPIOB,GPIO_PIN_13,GPIO_Pin_SET); //开启LDO

    Key_Init();  //最后再重新初始化按键 把CONFIRM按键功能恢复回来
    in_progress_flag = 0;
    in_sub_menu_flag = 0;
    Show_Main_Menu();
}

/**************************************************************************************
	函数名称：	System_Info

	函数功能：	显示系统信息

	入口参数：	无

	返回参数：	无
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
	函数名称：	Brightness_Setup

	函数功能：	设置屏幕亮度

	入口参数：	无

	返回参数：	无
**************************************************************************************/
void Brightness_Setup()
{
    Gui_DrawFont_GBK16(48,0,LIGHTBLUE,BLACK,"亮度设置");
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
    while(key_flag[2]==0) //返回键一直未被按下
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
	函数名称：	Enable_or_Disable_ESP

	函数功能：	启动或停止ESP8266

	入口参数：	无

	返回参数：	无
**************************************************************************************/
void Enable_or_Disable_ESP()
{
	if(ESP8266_Status == 0)
	{
		GPIO_WritePin(ESP8266_EN_PORT,ESP8266_EN_PIN,GPIO_Pin_SET);
		sub_menu_button[4][4]="停止8266  ";
	}
	else
	{
		GPIO_WritePin(ESP8266_EN_PORT,ESP8266_EN_PIN,GPIO_Pin_RESET);
		sub_menu_button[4][4]="启动8266  ";
	}
	in_progress_flag = 0;
	Show_Main_Menu();
}
