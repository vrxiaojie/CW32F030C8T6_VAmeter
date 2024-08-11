/**
* @file         adc.c
* @brief		����ģ��ת����ز�����Դ�ļ�
* @author		VRxiaojie
* @date		    2024-08-11
* @version	    v1.0
* @note 	 	��������	:		VRС��		https://space.bilibili.com/11526854 <br>
* 				��������Դ�㳡:	vrxiaojie	https://oshwhub.com/vrxiaojie/	
*/

#include "adc.h"

uint16_t adc_value[3]={0};
double bat_volume=0;

/**
* @brief		��ȡ����ʾ��ص���
* @param		��
* @return		bat_volume ��ص����ٷֱ�	
*/

uint16_t Battery_Volume(void)
{
	ADC_DeInit(); 	//����ǰ�Ƚ�ADCȥ��ʼ��������������ADCת���Ľ������
	/* GPIO���� */
	
	__RCC_GPIOA_CLK_ENABLE(); // ����GPIOAʱ��
	
	__RCC_ADC_CLK_ENABLE();	  // ����ADCʱ��
	
	PA07_ANALOG_ENABLE();	  // PA07�趨Ϊģ������
	
	/* ADC���� */
	
	ADC_InitTypeDef ADC_InitStructure;			// ADC��ʼ���ṹ��
    ADC_SingleChTypeDef ADC_SingleChStructure;	// ADC��ͨ��ת���ṹ��
	
	// ����ADC��ʼ���ṹ��
	ADC_InitStructure.ADC_OpMode = ADC_SingleChOneMode;  //��ͨ������ת��ģʽ
    ADC_InitStructure.ADC_ClkDiv = ADC_Clk_Div4; 		 // ʱ��Ƶ�� = PCLK / 4 = 64MHz / 4 = 16MHz
    ADC_InitStructure.ADC_SampleTime = ADC_SampTime5Clk; //5��ADCʱ������
    ADC_InitStructure.ADC_VrefSel = ADC_Vref_BGR2p5; 		 //2.5V�ο���ѹ
    ADC_InitStructure.ADC_InBufEn = ADC_BufDisable; 	 //�رո�����
    ADC_InitStructure.ADC_TsEn = ADC_TsDisable; 		 //�ر������¶ȴ�����
    ADC_InitStructure.ADC_DMAEn = ADC_DmaDisable; 		 //������DMA
    ADC_InitStructure.ADC_Align = ADC_AlignRight; 		 //ADCת������Ҷ���
    ADC_InitStructure.ADC_AccEn = ADC_AccDisable; 		 //ת������ۼӲ�ʹ��
	
	//���õ�ͨ��ת��ģʽ
    ADC_SingleChStructure.ADC_DiscardEn = ADC_DiscardNull;		// ��ͨ��ADCת������������
    ADC_SingleChStructure.ADC_Chmux = ADC_ExInputCH7;    		// ѡ��ADCת��ͨ����AIN7:PA07
    ADC_SingleChStructure.ADC_InitStruct = ADC_InitStructure;	// ADC��ʼ���ṹ��
	
	ADC_SingleChCommon(&ADC_SingleChStructure);		// ��ʼ������
	
    ADC_Enable(); //ADCʹ��
	
	ADC_SoftwareStartConvCmd(ENABLE);    //����ADCת��
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
	bat_volume = ((bat_ADC_value/4095.0*2.5*2) - 2.8+0.051)/(4.2-2.8); //�������4.2V������2.8V;У׼���0.051V
	return (uint16_t)(bat_volume*100);
	
}

/**
* @brief		��ʼ����ѹ�������ADC����
* @param		��
* @return		��	
*/

void Init_VAmeter_ADC(void)
{
	ADC_DeInit();
	
	ADC_SerialChTypeDef     ADC_sInitStruct;

    __RCC_ADC_CLK_ENABLE();     // ADCʱ��ʹ��
    __RCC_GPIOB_CLK_ENABLE();   // GPIOBʱ��ʹ��

    //����ADC����IO��  ADC_IN9->��ѹ0~3V;  ADC_IN11->��ѹ0~31V;  ADC_IN12->����0~1A
    PB01_ANALOG_ENABLE();	
	PB10_ANALOG_ENABLE();
	PB11_ANALOG_ENABLE();
    
    //����ת��
    ADC_sInitStruct.ADC_SqrEns = ADC_SqrEns02;
    ADC_sInitStruct.ADC_Sqr0Chmux = ADC_ExInputCH9;     	//ͨ��9����PB01
    ADC_sInitStruct.ADC_Sqr1Chmux = ADC_ExInputCH11;      	//ͨ��11����PB10
    ADC_sInitStruct.ADC_Sqr2Chmux = ADC_ExInputCH12;      	//ͨ��12����PB11
    
    ADC_sInitStruct.ADC_InitStruct.ADC_AccEn = ADC_AccDisable;   //ת������ۼӲ�ʹ��
    ADC_sInitStruct.ADC_InitStruct.ADC_Align = ADC_AlignRight;   //ADCת������Ҷ���
    ADC_sInitStruct.ADC_InitStruct.ADC_ClkDiv = ADC_Clk_Div128;   //PCLK 64MHz/128
    ADC_sInitStruct.ADC_InitStruct.ADC_DMAEn = ADC_DmaDisable;   //�ر�DMA����
    ADC_sInitStruct.ADC_InitStruct.ADC_SampleTime = ADC_SampTime5Clk;   //5��ADCʱ������
    ADC_sInitStruct.ADC_InitStruct.ADC_InBufEn = ADC_BufEnable;  //����������
    ADC_sInitStruct.ADC_InitStruct.ADC_VrefSel = ADC_Vref_BGR1p5;//�ο���ѹ1.5V
    
    ADC_sInitStruct.ADC_InitStruct.ADC_TsEn = ADC_TsEnable;    //�����¶ȴ���������
    ADC_sInitStruct.ADC_InitStruct.ADC_OpMode = ADC_SerialChScanMode;   //����ɨ��ģʽ��3��ͨ��ͬʱת��һ��
    
    ADC_SerialChScanModeCfg(&ADC_sInitStruct);
    ADC_ClrAccResult(); //��0�ۼ�ֵ
    
    ADC_Enable();    // ʹ��ADC
}

/**
* @brief		ADC��ֵ�˲�
* @param		times ADC��ȡֵ�Ĵ���
* @param		idx ADC�����±�
* @return		ADC_value ADC��ֵ
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
		
        if(tempvalue == 0)			   // ���ֵ��0�����ȡʧ�ܣ�times-1��
		{
			times--;
			continue;
		}
		value += tempvalue;			   // �ۼ����� 
    }
    return value/times/1.0;			   // ƽ��
}

/**
* @brief		��ȡһ��ADCֵ
* @param		��
* @return		��
*/
/**********************************************************
 * �� �� �� �ƣ�ADC_GET
 * �� �� �� �ܣ���ȡһ��ADCֵ
 * �� �� �� ������
 * �� �� �� �أ���
**********************************************************/
void ADC_GET(void)
{
	ADC_SoftwareStartConvCmd(ENABLE);    //����ADCת��
	//���λ�ȡADCֵ
	for(uint8_t i = 0;i<3;i++)
		adc_value[i] = Get_Adc_Average(10,i);
}
