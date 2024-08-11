/**
* @file         gpio.c
* @brief		GPIO相关操作源文件
* @author		VRxiaojie
* @date		    2024-08-11
* @version	    v1.0
* @note 	 	哔哩哔哩	:		VR小杰		https://space.bilibili.com/11526854 <br>
* 				嘉立创开源广场:	vrxiaojie	https://oshwhub.com/vrxiaojie/	
*/

#include "gpio.h"

/**
* @brief		GPIOA中断处理函数
* @param		无
* @return		无
*/

void GPIOA_IRQHandler(void)
{
    if(CW_GPIOA->ISR_f.PIN10)

    {
        GPIOA_INTFLAG_CLR(bv10);//清除CW_GPIO中断标志
    }

}

/**
* @brief		初始化用户定义的GPIO端口
* @param		无
* @return		无
*/
void GPIO_User_Init()
{
    GPIO_InitTypeDef	GPIO_InitStruct; // GPIO初始化结构体

    __RCC_GPIOA_CLK_ENABLE();// 使能GPIO时钟
    __RCC_GPIOB_CLK_ENABLE();
    __RCC_GPIOC_CLK_ENABLE();

    GPIO_InitStruct.Pins = ESP8266_EN_PIN;			// PB14作为ESP8266的启停开关
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;		//推挽输出
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;		// 输出速度高
    GPIO_Init(ESP8266_EN_PORT, &GPIO_InitStruct);			// 初始化

    GPIO_WritePin(ESP8266_EN_PORT,ESP8266_EN_PIN,GPIO_Pin_RESET); //默认关闭ESP8266

    GPIO_InitStruct.Pins = ESP8266_PIN;				// PB15作为检测ESP8266状态的引脚
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;			//浮空输入
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;		// 输出速度高
    GPIO_Init(ESP8266_PORT, &GPIO_InitStruct);		// 初始化
	
	GPIO_InitStruct.Pins = GPIO_PIN_13;				// PB13作为LDO的启停开关
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;		//推挽输出
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;		// 输出速度高
    GPIO_Init(CW_GPIOB, &GPIO_InitStruct);			// 初始化
	GPIO_WritePin(CW_GPIOB,GPIO_PIN_13,GPIO_Pin_SET);  //默认开启LDO
	
	GPIO_InitStruct.Pins = SCL_PIN|SDA_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	GPIO_Init(CW_GPIOA, &GPIO_InitStruct);
}

