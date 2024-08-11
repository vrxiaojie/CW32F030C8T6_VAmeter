/**
* @file         adc.c
* @brief		配置模数转换相关操作的源文件
* @author		VRxiaojie
* @date		    2024-08-11
* @version	    v1.0
* @note 	 	哔哩哔哩	:		VR小杰		https://space.bilibili.com/11526854 <br>
* 				嘉立创开源广场:	vrxiaojie	https://oshwhub.com/vrxiaojie/	
*/

#include "adc.h"

uint16_t adc_value[3]={0};
double bat_volume=0;

/**
* @brief		获取并显示电池电量
* @param		无
* @return		bat_volume 电池电量百分比	
*/

uint16_t Battery_Volume(void)
{
	ADC_DeInit(); 	//测量前先将ADC去初始化，以免有其他ADC转换的结果干扰
	/* GPIO配置 */
	
	__RCC_GPIOA_CLK_ENABLE(); // 开启GPIOA时钟
	
	__RCC_ADC_CLK_ENABLE();	  // 开启ADC时钟
	
	PA07_ANALOG_ENABLE();	  // PA07设定为模拟输入
	
	/* ADC配置 */
	
	ADC_InitTypeDef ADC_InitStructure;			// ADC初始化结构体
    ADC_SingleChTypeDef ADC_SingleChStructure;	// ADC单通道转换结构体
	
	// 配置ADC初始化结构体
	ADC_InitStructure.ADC_OpMode = ADC_SingleChOneMode;  //单通道单次转换模式
    ADC_InitStructure.ADC_ClkDiv = ADC_Clk_Div4; 		 // 时钟频率 = PCLK / 4 = 64MHz / 4 = 16MHz
    ADC_InitStructure.ADC_SampleTime = ADC_SampTime5Clk; //5个ADC时钟周期
    ADC_InitStructure.ADC_VrefSel = ADC_Vref_BGR2p5; 		 //2.5V参考电压
    ADC_InitStructure.ADC_InBufEn = ADC_BufDisable; 	 //关闭跟随器
    ADC_InitStructure.ADC_TsEn = ADC_TsDisable; 		 //关闭内置温度传感器
    ADC_InitStructure.ADC_DMAEn = ADC_DmaDisable; 		 //不触发DMA
    ADC_InitStructure.ADC_Align = ADC_AlignRight; 		 //ADC转换结果右对齐
    ADC_InitStructure.ADC_AccEn = ADC_AccDisable; 		 //转换结果累加不使能
	
	//配置单通道转换模式
    ADC_SingleChStructure.ADC_DiscardEn = ADC_DiscardNull;		// 单通道ADC转换结果溢出保存
    ADC_SingleChStructure.ADC_Chmux = ADC_ExInputCH7;    		// 选择ADC转换通道，AIN7:PA07
    ADC_SingleChStructure.ADC_InitStruct = ADC_InitStructure;	// ADC初始化结构体
	
	ADC_SingleChCommon(&ADC_SingleChStructure);		// 初始化配置
	
    ADC_Enable(); //ADC使能
	
	ADC_SoftwareStartConvCmd(ENABLE);    //启动ADC转换
	uint32_t temp=0;
	uint32_t bat_ADC_value=0;
	for(uint8_t i = 0;i<10;i++)
	{
		temp = ADC_GetConversionValue();
		if(temp==0)
		{
			i--;
			continue;
		}
		bat_ADC_value += temp;
	}
	bat_ADC_value/=10;
	bat_volume = ((bat_ADC_value/4095.0*2.5*2) - 2.8+0.051)/(4.2-2.8); //电池满电4.2V，亏电2.8V;校准后加0.051V
	return (uint16_t)(bat_volume*100);
	
}

/**
* @brief		初始化电压电流表的ADC配置
* @param		无
* @return		无	
*/

void Init_VAmeter_ADC(void)
{
	ADC_DeInit();
	
	ADC_SerialChTypeDef     ADC_sInitStruct;

    __RCC_ADC_CLK_ENABLE();     // ADC时钟使能
    __RCC_GPIOB_CLK_ENABLE();   // GPIOB时钟使能

    //配置ADC输入IO口  ADC_IN9->电压0~3V;  ADC_IN11->电压0~31V;  ADC_IN12->电流0~1A
    PB01_ANALOG_ENABLE();	
	PB10_ANALOG_ENABLE();
	PB11_ANALOG_ENABLE();
    
    //序列转换
    ADC_sInitStruct.ADC_SqrEns = ADC_SqrEns02;
    ADC_sInitStruct.ADC_Sqr0Chmux = ADC_ExInputCH9;     	//通道9输入PB01
    ADC_sInitStruct.ADC_Sqr1Chmux = ADC_ExInputCH11;      	//通道11输入PB10
    ADC_sInitStruct.ADC_Sqr2Chmux = ADC_ExInputCH12;      	//通道12输入PB11
    
    ADC_sInitStruct.ADC_InitStruct.ADC_AccEn = ADC_AccDisable;   //转换结果累加不使能
    ADC_sInitStruct.ADC_InitStruct.ADC_Align = ADC_AlignRight;   //ADC转换结果右对齐
    ADC_sInitStruct.ADC_InitStruct.ADC_ClkDiv = ADC_Clk_Div128;   //PCLK 64MHz/128
    ADC_sInitStruct.ADC_InitStruct.ADC_DMAEn = ADC_DmaDisable;   //关闭DMA传输
    ADC_sInitStruct.ADC_InitStruct.ADC_SampleTime = ADC_SampTime5Clk;   //5个ADC时钟周期
    ADC_sInitStruct.ADC_InitStruct.ADC_InBufEn = ADC_BufEnable;  //开启跟随器
    ADC_sInitStruct.ADC_InitStruct.ADC_VrefSel = ADC_Vref_BGR1p5;//参考电压1.5V
    
    ADC_sInitStruct.ADC_InitStruct.ADC_TsEn = ADC_TsEnable;    //内置温度传感器禁用
    ADC_sInitStruct.ADC_InitStruct.ADC_OpMode = ADC_SerialChScanMode;   //序列扫描模式：3个通道同时转换一次
    
    ADC_SerialChScanModeCfg(&ADC_sInitStruct);
    ADC_ClrAccResult(); //清0累加值
    
    ADC_Enable();    // 使能ADC
}

/**
* @brief		ADC均值滤波
* @param		times ADC获取值的次数
* @param		idx ADC序列下标
* @return		ADC_value ADC数值
*/

double Get_Adc_Average(uint8_t times,uint8_t idx)
{
    uint16_t value = 0;
    uint8_t t;
    
    for(t=0;t<times;t++)
    {
		uint16_t tempvalue = 0;
		switch(idx)
		{
			case 0:
				ADC_GetSqr0Result(&tempvalue);
				break;
			case 1:
				ADC_GetSqr1Result(&tempvalue);
				break;
			case 2:
				ADC_GetSqr2Result(&tempvalue);
				break;
		}
		
        if(tempvalue == 0)			   // 如果值是0，则读取失败，times-1。
		{
			times--;
			continue;
		}
		value += tempvalue;			   // 累加数据 
    }
    return value/times/1.0;			   // 平均
}

/**
* @brief		读取一次ADC值
* @param		无
* @return		无
*/
/**********************************************************
 * 函 数 名 称：ADC_GET
 * 函 数 功 能：读取一次ADC值
 * 传 入 参 数：无
 * 函 数 返 回：无
**********************************************************/
void ADC_GET(void)
{
	ADC_SoftwareStartConvCmd(ENABLE);    //启动ADC转换
	//依次获取ADC值
	for(uint8_t i = 0;i<3;i++)
		adc_value[i] = Get_Adc_Average(10,i);
}
