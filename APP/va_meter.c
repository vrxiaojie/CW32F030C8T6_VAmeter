<<<<<<< HEAD
/**
* @file         va_meter.c
* @brief		电压电流表相关操作的源文件
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
#include "va_meter.h"
#include <math.h>
#include <stdlib.h>
#include "menu.h"

<<<<<<< HEAD
uint8_t va_meter_style; ///<显示样式选择：0 - 图形模式  1 - 纯数字模式
=======
uint8_t va_meter_style=5; //显示样式选择：0 - 图形模式  1 - 纯数字模式
>>>>>>> 0dfd7a7e8c0db99ec07f30fe2edd2e84eaf20952
uint8_t time=2,last_time,last_voltage_value;

short voltage_num[5]= {0};
short current_num[5]= {0};
double voltage_value = 0,current_value=0;
<<<<<<< HEAD
#define MAX_VOLTAGE_Y 6 ///<电压纵坐标最大值6V
#define MAX_CURRENT_Y 1000  ///<电流纵坐标最大值1000mA

/**
* @brief		绘制电压曲线、电流柱形图
* @param		无
* @return		无
*/

=======
#define MAX_VOLTAGE_Y 6 //电压纵坐标最大值6V
#define MAX_CURRENT_Y 1000  //电流纵坐标最大值1000mA

/**************************************************************************************
	函数名称：	Draw_Value_Line

	函数功能：	绘制电压曲线、电流柱形图

	入口参数：	无

	返回参数：	无
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
    //画图范围纵坐标20-110，横坐标2-158
    if(voltage_value>MAX_VOLTAGE_Y*10 || current_value>MAX_CURRENT_Y)
        return ;
	
	//先绘制电流柱状图，如果和电压曲线图颠倒绘制顺序，则会遮住曲线图
	Gui_DrawLine(time,110-(int)(90*current_value/MAX_CURRENT_Y),time,110,BLUE);
	
	//再绘制电压曲线图
    if(time==2)
	{
        Gui_DrawPoint(time,110-(int)(90*voltage_value/10/MAX_VOLTAGE_Y),GREEN); 
	}
    else
    {
        Gui_DrawLine(last_time,110-(int)(90*last_voltage_value/10/MAX_VOLTAGE_Y),time,110-(int)(90*voltage_value/10/MAX_VOLTAGE_Y),GREEN); 	//voltage
	}

    Gui_DrawLine(time+1,20,time+1,110,BLACK); //擦除下一时刻的值
    time++;
    last_voltage_value = voltage_value;
    last_time = time;
}

<<<<<<< HEAD
/**
* @brief		启动电压电流表
* @param		idx 0为图形模式；1为数字模式
* @return		无
*/

=======
/**************************************************************************************
	函数名称：	Launch_VA_Meter

	函数功能：	启动电压电流表

	入口参数：	idx——0为图形模式；1为数字模式

	返回参数：	无
**************************************************************************************/
>>>>>>> 0dfd7a7e8c0db99ec07f30fe2edd2e84eaf20952
void Launch_VA_Meter(uint8_t idx)
{
    Lcd_Clear(BLACK);
    Init_VAmeter_ADC();
	key_flag[2]=0;
	uint8_t send_wait = 100;
	double mqtt_volt,mqtt_curr;
    //初始化不同样式的界面
    switch(idx)
    {
    case 0:
        Gui_DrawFont_GBK16(40,0,LIGHTBLUE,BLACK,"电压电流表");
        Gui_DrawLine(0,18,160,18,WHITE);
        Gui_DrawLine(0,111,160,111,WHITE);
        time = 2;
        va_meter_style = 0;
        break;
    case 1:
        Gui_DrawFont_GBK16(40,0,LIGHTBLUE,BLACK,"电压电流表");
        Gui_DrawFont_Num32(64,24,YELLOW,BLACK,10); //显示电压的小数点
        Gui_DrawFont_GBK16(144,38,YELLOW,BLACK,"V"); //显示电压的单位
        Gui_DrawFont_Num32(96,80,YELLOW,BLACK,10); //显示电流的小数点
        Gui_DrawFont_GBK16(144,80,YELLOW,BLACK,"m"); //显示电压的单位 m
        Gui_DrawFont_GBK16(144,96,YELLOW,BLACK,"A"); //显示电压的单位 A
        va_meter_style = 1;
        break;
    }

    while(key_flag[2]==0)  //向左按键未被按下，一直在循环内
    {
        ADC_GET();
		
		//处理电压数据
		if(adc_value[0]>=4090)  //当电压值大于3V时，换挡到0~31V
		{
			voltage_value = (adc_value[1]/4095.0 * 1.5 * 21)-0.075;   //参考电压是1.5V，分压比1:20 经校准比正常值高0.075V，故减去0.075
			Gui_DrawFont_GBK16(128,0,RED,BLACK,"3V");
		}
		else	//电压值小于3V时，换挡到0~3V
		{
			voltage_value = (adc_value[0]/4095.0 * 1.5 * 2);	//参考电压是1.5V，分压比1:1 
			Gui_DrawFont_GBK16(128,0,GREEN,BLACK,"3V");
		}
		mqtt_volt = voltage_value;
        voltage_value *= 100;	//处理以显示小数点后2位
        for(uint8_t i = 0; i<4; i++)
            voltage_num[3-i]=(int)(voltage_value/(pow(10,i))) %10;
		
		//处理电流数据
        current_value = ((adc_value[2]/4095.0 * 1.5)/0.2 * 1000)-10.8;   //参考电压是1.5V，采样电阻0.2ohm，电流单位mA，经校准比理论值高10.8mA
		if(current_value < 0 || current_value > 7000) //未采集到正确数据	
			current_value = 0;
		mqtt_curr = current_value;
		current_value *= 10;  	//处理以显示小数点后1位
		for(uint8_t i = 0; i<4; i++)
            current_num[3-i]=(int)(current_value/(pow(10,i))) %10;
		//MQTT发送间隔  4-->-约等于0.3s
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
            //电压
            for(uint8_t i = 0; i<2; i++)
                Gui_DrawFont_Num32(i*32,24,ORANGE,BLACK,voltage_num[i]);
            for(uint8_t i = 2; i<4; i++)
            {
                Gui_DrawFont_Num32(16+i*32,24,BLUE,BLACK,voltage_num[i]);
            }
            //电流
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
* @brief		电压电流表发送数据到MQTT消息队列
* @param		volt 电压值
* @param		curr 电流值
* @return		无
*/

=======
/**************************************************************************************
	函数名称：	VAmeter_Mqtt_Send_Data

	函数功能：	电压电流表发送数据到MQTT消息队列

	入口参数：	volt——电压值
				curr——电流值

	返回参数：	无
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
