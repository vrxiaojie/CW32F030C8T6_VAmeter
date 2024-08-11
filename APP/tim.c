<<<<<<< HEAD
/**
* @file         tim.c
* @brief		定时器相关的源文件
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
#include "tim.h"
#include "esp8266_cmd.h"
#include "menu.h"
#include "Lcd_Driver.h"

<<<<<<< HEAD
uint8_t bat_cnt=0;  ///<更新电池电量时间计数

/**
* @brief		定时器BTIM1初始化函数  10ms 中断一次
* @param		无
* @return		无	
*/

=======
uint8_t bat_cnt=0;

/******************************************************************
 * 函 数 名 称：BTIM1_Init
 * 函 数 说 明：定时器BTIM1初始化函数  10ms 中断一次
 * 函 数 形 参：无
 * 函 数 返 回：无
******************************************************************/
>>>>>>> 0dfd7a7e8c0db99ec07f30fe2edd2e84eaf20952
void BTIM1_Init(void)
{
    // 使能BTIM1的时钟
    __RCC_BTIM_CLK_ENABLE();

    // 禁止中断，以安全地配置NVIC
    __disable_irq();

    // 开启BTIM1中断，并关联到NVIC
    NVIC_EnableIRQ(BTIM1_IRQn);

    // 允许中断，恢复中断状态
    __enable_irq();

    // 定义并初始化定时器时间基准结构体
    BTIM_TimeBaseInitTypeDef BTIM_TimeBaseInitStruct;

    // 配置定时器模式、周期和预分频器
    BTIM_TimeBaseInitStruct.BTIM_Mode = BTIM_Mode_TIMER; // 设置为定时器模式
    BTIM_TimeBaseInitStruct.BTIM_Period = 625-1;   // 设置周期 16us*625=10ms，即每10ms中断一次
    BTIM_TimeBaseInitStruct.BTIM_Prescaler = BTIM_PRS_DIV1024; // 时钟64MHz，预分频器设置为1024，分频为62.5kHz，即周期为16us
    // 使用上述配置初始化定时器BTIM1
    BTIM_TimeBaseInit(CW_BTIM1, &BTIM_TimeBaseInitStruct);

    // 使能BTIM1的溢出中断
    BTIM_ITConfig(CW_BTIM1, BTIM_IT_OV, ENABLE);
    // 启动定时器BTIM1
    BTIM_Cmd(CW_BTIM1, ENABLE);
}

<<<<<<< HEAD
/**
* @brief		定时器BTIM1中断服务函数，用于判断按键状态
* @param		无
* @return		无	
*/

=======

/******************************************************************
 * 函 数 名 称：BTIM1_IRQHandler
 * 函 数 说 明：定时器BTIM1中断服务函数，用处是判断按键状态
 * 函 数 形 参：无
 * 函 数 返 回：无
******************************************************************/
>>>>>>> 0dfd7a7e8c0db99ec07f30fe2edd2e84eaf20952
void BTIM1_IRQHandler(void)
{
    // 判断是否为溢出中断
    if (BTIM_GetITStatus(CW_BTIM1, BTIM_IT_OV))
    {
		//检查按键状态函数放进了key.c
		Check_Key_Status();
		// 清除溢出中断标志
        BTIM_ClearITPendingBit(CW_BTIM1, BTIM_IT_OV);
    }
}

<<<<<<< HEAD
/**
* @brief		定时器BTIM2初始化函数  1s 中断一次
* @param		无
* @return		无	
*/

=======
/******************************************************************
 * 函 数 名 称：BTIM2_Init
 * 函 数 说 明：定时器BTIM1初始化函数  1s 中断一次
 * 函 数 形 参：无
 * 函 数 返 回：无
******************************************************************/
>>>>>>> 0dfd7a7e8c0db99ec07f30fe2edd2e84eaf20952
void BTIM2_Init(void)
{
    // 使能BTIM2的时钟
    __RCC_BTIM_CLK_ENABLE();

    // 禁止中断，以安全地配置NVIC
    __disable_irq();

    // 开启BTIM2中断，并关联到NVIC
    NVIC_EnableIRQ(BTIM2_IRQn);

    // 允许中断，恢复中断状态
    __enable_irq();

    // 定义并初始化定时器时间基准结构体
    BTIM_TimeBaseInitTypeDef BTIM_TimeBaseInitStruct;

    // 配置定时器模式、周期和预分频器
    BTIM_TimeBaseInitStruct.BTIM_Mode = BTIM_Mode_TIMER; // 设置为定时器模式
    BTIM_TimeBaseInitStruct.BTIM_Period = 15625-1;   // 设置周期 64us*15625=1s，即每1s中断一次
    BTIM_TimeBaseInitStruct.BTIM_Prescaler = BTIM_PRS_DIV4096; // 时钟64MHz，预分频器设置为4096，分频为15.625kHz，即周期为64us
    // 使用上述配置初始化定时器BTIM2
    BTIM_TimeBaseInit(CW_BTIM2, &BTIM_TimeBaseInitStruct);

    // 使能BTIM2的溢出中断
    BTIM_ITConfig(CW_BTIM2, BTIM_IT_OV, ENABLE);
    // 启动定时器BTIM2
    BTIM_Cmd(CW_BTIM2, ENABLE);
}

<<<<<<< HEAD
/**
* @brief		定时器BTIM2中断服务函数，用于更新状态栏
* @param		无
* @return		无	
*/

=======

/******************************************************************
 * 函 数 名 称：BTIM2_Init
 * 函 数 说 明：定时器BTIM2中断服务函数，用于更新状态栏
 * 函 数 形 参：无
 * 函 数 返 回：无
******************************************************************/
>>>>>>> 0dfd7a7e8c0db99ec07f30fe2edd2e84eaf20952
void BTIM2_IRQHandler(void)
{
    // 判断是否为溢出中断
    if (BTIM_GetITStatus(CW_BTIM2, BTIM_IT_OV))
    {
        //当ESP8266使能引脚高电平，且ESP8266检测引脚也是高电平时，说明ESP8266已连接
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
        // 清除溢出中断标志
        BTIM_ClearITPendingBit(CW_BTIM2, BTIM_IT_OV);
    }
}

<<<<<<< HEAD
/**
* @brief		定时器GTIM3初始化函数，用于屏幕PWM调光
* @param		无
* @return		无	
*/

=======

/******************************************************************
 * 函 数 名 GTIM3_PWM_Init
 * 函 数 说 明：定时器GTIM3初始化函数，用于屏幕PWM调光
 * 函 数 形 参：无
 * 函 数 返 回：无
******************************************************************/
>>>>>>> 0dfd7a7e8c0db99ec07f30fe2edd2e84eaf20952
void GTIM3_PWM_Init(void)
{
	PC15_AFx_GTIM3CH2();						// 使用复用功能，复用为通用定时器3通道2
	/*********** GTIM3配置 ***********/
	GTIM_InitTypeDef GTIM_InitStruct;  // 通用定时器初始化结构体
	
    __RCC_GTIM3_CLK_ENABLE();		   // 使能通用定时器1时钟
	
    GTIM_InitStruct.Mode = GTIM_MODE_TIME;				 // 定时器模式
    GTIM_InitStruct.OneShotMode = GTIM_COUNT_CONTINUE;	 // 连续计数模式
    GTIM_InitStruct.Prescaler = GTIM_PRESCALER_DIV512;    // DCLK = PCLK / 128 = 64MHz/512 = 125KHz
    GTIM_InitStruct.ReloadValue = 100;	 			 	 // 重装载值设置  PWM:1.25KHz
    GTIM_InitStruct.ToggleOutState = DISABLE;			 // 输出翻转功能
    GTIM_TimeBaseInit(CW_GTIM3, &GTIM_InitStruct);		 // 初始化
	
    GTIM_OCInit(CW_GTIM3, GTIM_CHANNEL2, GTIM_OC_OUTPUT_PWM_HIGH); // 配置输出比较通道3为PWM模式
	GTIM_SetCompare2(CW_GTIM3, lcd_brightness);  //设置初始占空比为lcd_brightness/100 = 50/100 = 50%
    GTIM_Cmd(CW_GTIM3, ENABLE);     // 使能定时器
}

