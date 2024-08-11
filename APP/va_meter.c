<<<<<<< HEAD
/**
* @file         va_meter.c
* @brief		��ѹ��������ز�����Դ�ļ�
* @author		VRxiaojie
* @date		    2024-08-11
* @version	    v1.0
* @note 	 	��������	:		VRС��		https://space.bilibili.com/11526854 <br>
* 				��������Դ�㳡:	vrxiaojie	https://oshwhub.com/vrxiaojie/	
*/

=======
/*
 * ���������˺�		: 	VRС��      	https://space.bilibili.com/11526854
 * ��������Դ�㳡	: 	vrxiaojie   https://oshwhub.com/vrxiaojie/
 *
 *   Date           Author
 * 2024-08-10     VRxiaojie
 */
>>>>>>> 0dfd7a7e8c0db99ec07f30fe2edd2e84eaf20952
#include "va_meter.h"
#include <math.h>
#include <stdlib.h>
#include "menu.h"

<<<<<<< HEAD
uint8_t va_meter_style; ///<��ʾ��ʽѡ��0 - ͼ��ģʽ  1 - ������ģʽ
=======
uint8_t va_meter_style=5; //��ʾ��ʽѡ��0 - ͼ��ģʽ  1 - ������ģʽ
>>>>>>> 0dfd7a7e8c0db99ec07f30fe2edd2e84eaf20952
uint8_t time=2,last_time,last_voltage_value;

short voltage_num[5]= {0};
short current_num[5]= {0};
double voltage_value = 0,current_value=0;
<<<<<<< HEAD
#define MAX_VOLTAGE_Y 6 ///<��ѹ���������ֵ6V
#define MAX_CURRENT_Y 1000  ///<�������������ֵ1000mA

/**
* @brief		���Ƶ�ѹ���ߡ���������ͼ
* @param		��
* @return		��
*/

=======
#define MAX_VOLTAGE_Y 6 //��ѹ���������ֵ6V
#define MAX_CURRENT_Y 1000  //�������������ֵ1000mA

/**************************************************************************************
	�������ƣ�	Draw_Value_Line

	�������ܣ�	���Ƶ�ѹ���ߡ���������ͼ

	��ڲ�����	��

	���ز�����	��
**************************************************************************************/
>>>>>>> 0dfd7a7e8c0db99ec07f30fe2edd2e84eaf20952
void Draw_Value_Line()
{
    if(time==158)
    {
        time=2;
        Gui_DrawLine(2,20,2,110,BLACK);
    }

    voltage_value=(int)voltage_value/10;
	current_value=(int)current_value/10;
    //��ͼ��Χ������20-110��������2-158
    if(voltage_value>MAX_VOLTAGE_Y*10 || current_value>MAX_CURRENT_Y)
        return ;
	
	//�Ȼ��Ƶ�����״ͼ������͵�ѹ����ͼ�ߵ�����˳�������ס����ͼ
	Gui_DrawLine(time,110-(int)(90*current_value/MAX_CURRENT_Y),time,110,BLUE);
	
	//�ٻ��Ƶ�ѹ����ͼ
    if(time==2)
	{
        Gui_DrawPoint(time,110-(int)(90*voltage_value/10/MAX_VOLTAGE_Y),GREEN); 
	}
    else
    {
        Gui_DrawLine(last_time,110-(int)(90*last_voltage_value/10/MAX_VOLTAGE_Y),time,110-(int)(90*voltage_value/10/MAX_VOLTAGE_Y),GREEN); 	//voltage
	}

    Gui_DrawLine(time+1,20,time+1,110,BLACK); //������һʱ�̵�ֵ
    time++;
    last_voltage_value = voltage_value;
    last_time = time;
}

<<<<<<< HEAD
/**
* @brief		������ѹ������
* @param		idx 0Ϊͼ��ģʽ��1Ϊ����ģʽ
* @return		��
*/

=======
/**************************************************************************************
	�������ƣ�	Launch_VA_Meter

	�������ܣ�	������ѹ������

	��ڲ�����	idx����0Ϊͼ��ģʽ��1Ϊ����ģʽ

	���ز�����	��
**************************************************************************************/
>>>>>>> 0dfd7a7e8c0db99ec07f30fe2edd2e84eaf20952
void Launch_VA_Meter(uint8_t idx)
{
    Lcd_Clear(BLACK);
    Init_VAmeter_ADC();
	key_flag[2]=0;
	uint8_t send_wait = 100;
	double mqtt_volt,mqtt_curr;
    //��ʼ����ͬ��ʽ�Ľ���
    switch(idx)
    {
    case 0:
        Gui_DrawFont_GBK16(40,0,LIGHTBLUE,BLACK,"��ѹ������");
        Gui_DrawLine(0,18,160,18,WHITE);
        Gui_DrawLine(0,111,160,111,WHITE);
        time = 2;
        va_meter_style = 0;
        break;
    case 1:
        Gui_DrawFont_GBK16(40,0,LIGHTBLUE,BLACK,"��ѹ������");
        Gui_DrawFont_Num32(64,24,YELLOW,BLACK,10); //��ʾ��ѹ��С����
        Gui_DrawFont_GBK16(144,38,YELLOW,BLACK,"V"); //��ʾ��ѹ�ĵ�λ
        Gui_DrawFont_Num32(96,80,YELLOW,BLACK,10); //��ʾ������С����
        Gui_DrawFont_GBK16(144,80,YELLOW,BLACK,"m"); //��ʾ��ѹ�ĵ�λ m
        Gui_DrawFont_GBK16(144,96,YELLOW,BLACK,"A"); //��ʾ��ѹ�ĵ�λ A
        va_meter_style = 1;
        break;
    }

    while(key_flag[2]==0)  //���󰴼�δ�����£�һֱ��ѭ����
    {
        ADC_GET();
		
		//�����ѹ����
		if(adc_value[0]>=4090)  //����ѹֵ����3Vʱ��������0~31V
		{
			voltage_value = (adc_value[1]/4095.0 * 1.5 * 21)-0.075;   //�ο���ѹ��1.5V����ѹ��1:20 ��У׼������ֵ��0.075V���ʼ�ȥ0.075
			Gui_DrawFont_GBK16(128,0,RED,BLACK,"3V");
		}
		else	//��ѹֵС��3Vʱ��������0~3V
		{
			voltage_value = (adc_value[0]/4095.0 * 1.5 * 2);	//�ο���ѹ��1.5V����ѹ��1:1 
			Gui_DrawFont_GBK16(128,0,GREEN,BLACK,"3V");
		}
		mqtt_volt = voltage_value;
        voltage_value *= 100;	//��������ʾС�����2λ
        for(uint8_t i = 0; i<4; i++)
            voltage_num[3-i]=(int)(voltage_value/(pow(10,i))) %10;
		
		//�����������
        current_value = ((adc_value[2]/4095.0 * 1.5)/0.2 * 1000)-10.8;   //�ο���ѹ��1.5V����������0.2ohm��������λmA����У׼������ֵ��10.8mA
		if(current_value < 0 || current_value > 7000) //δ�ɼ�����ȷ����	
			current_value = 0;
		mqtt_curr = current_value;
		current_value *= 10;  	//��������ʾС�����1λ
		for(uint8_t i = 0; i<4; i++)
            current_num[3-i]=(int)(current_value/(pow(10,i))) %10;
		//MQTT���ͼ��  4-->-Լ����0.3s
		send_wait++;
		if(send_wait>4)
		{
			VAmeter_Mqtt_Send_Data(mqtt_volt,mqtt_curr);
			send_wait=0;
		}
		
        switch(va_meter_style)
        {
        case 0:
            for(uint8_t i = 0; i<2; i++)
			{
                Gui_DrawFont_Num16(i*8,112,GREEN,BLACK,voltage_num[i]);
			}
            Gui_DrawFont_GBK16(16,112,ORANGE,BLACK,".");
            for(uint8_t i = 2; i<4; i++)
            {
                Gui_DrawFont_Num16(8+i*8,112,GREEN,BLACK,voltage_num[i]);
            }
            Gui_DrawFont_GBK16(40,112,ORANGE,BLACK,"V");
			
			for(uint8_t i = 0; i<3; i++)
            {
                Gui_DrawFont_Num16(60+i*8,112,BLUE,BLACK,current_num[i]);
            }
			Gui_DrawFont_GBK16(84,112,ORANGE,BLACK,".");
            Gui_DrawFont_Num16(92,112,BLUE,BLACK,current_num[3]);
			Gui_DrawFont_GBK16(100,112,ORANGE,BLACK,"mA");
			
            Draw_Value_Line();
            break;
        case 1:
            //��ѹ
            for(uint8_t i = 0; i<2; i++)
                Gui_DrawFont_Num32(i*32,24,ORANGE,BLACK,voltage_num[i]);
            for(uint8_t i = 2; i<4; i++)
            {
                Gui_DrawFont_Num32(16+i*32,24,BLUE,BLACK,voltage_num[i]);
            }
            //����
            for(uint8_t i = 0; i<3; i++)
            {
                Gui_DrawFont_Num32(i*32,80,ORANGE,BLACK,current_num[i]);
            }
            Gui_DrawFont_Num32(112,80,BLUE,BLACK,current_num[3]);
            break;
        }
        delay_ms(5);
    }
	Lcd_Clear(BLACK);
	Show_Main_Menu();
	in_progress_flag = 0;
	key_flag[2]=0;
}

<<<<<<< HEAD
/**
* @brief		��ѹ�����������ݵ�MQTT��Ϣ����
* @param		volt ��ѹֵ
* @param		curr ����ֵ
* @return		��
*/

=======
/**************************************************************************************
	�������ƣ�	VAmeter_Mqtt_Send_Data

	�������ܣ�	��ѹ�����������ݵ�MQTT��Ϣ����

	��ڲ�����	volt������ѹֵ
				curr��������ֵ

	���ز�����	��
**************************************************************************************/
>>>>>>> 0dfd7a7e8c0db99ec07f30fe2edd2e84eaf20952
void VAmeter_Mqtt_Send_Data(double volt,double curr)
{
	if(mqtt_status == 0)
		return ;
	double pwr = volt*curr;
	char *data = malloc(150);
	char t[10] = {0};
	strcpy(data,"{\\\"volt\\\":");
	num2char(t,volt,log10(volt)+1,2);
	strcat(data,t);
	memset(t,0,10);
	strcat(data,"\\,\\\"curr\\\":");
	num2char(t,curr,log10(curr)+1,1);
	strcat(data,t);
	memset(t,0,10);
	
	strcat(data,"\\,\\\"pwr\\\":");
	num2char(t,pwr,log10(pwr)+1,1);
	strcat(data,t);
	free(t);
	
	strcat(data,"}");
	MQTT_Publish(data);
	free(data);
}
