<<<<<<< HEAD
/**
* @file         sht40.c
* @brief		SHT40温湿度计的源文件
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
#include "sht40.h"

static void SDA_OUT(void){
    GPIO_InitTypeDef SOFT_IIC_GPIO_STRUCT;
    SOFT_IIC_GPIO_STRUCT.Mode = GPIO_MODE_OUTPUT_OD;
    SOFT_IIC_GPIO_STRUCT.Pins = SDA_PIN;
    SOFT_IIC_GPIO_STRUCT.Speed = GPIO_SPEED_HIGH;

    GPIO_Init(SDA_PORT, &SOFT_IIC_GPIO_STRUCT);
}

static void SDA_IN(void)
{
    GPIO_InitTypeDef SOFT_IIC_GPIO_STRUCT;
    SOFT_IIC_GPIO_STRUCT.Mode = GPIO_MODE_INPUT;
    SOFT_IIC_GPIO_STRUCT.Pins = SDA_PIN;
    SOFT_IIC_GPIO_STRUCT.Speed = GPIO_SPEED_HIGH;

    GPIO_Init(SDA_PORT, &SOFT_IIC_GPIO_STRUCT);
}

void IIC_Delay(void) {
	delay_us(5);
}

void IIC_Start(void) {
    SDA_OUT();
    SDA_HIGH();
    SCL_HIGH();
    IIC_Delay();
    SDA_LOW();
    IIC_Delay();
    SCL_LOW();
}


void IIC_Stop(void) {
    SDA_OUT();
    SCL_LOW();
    SDA_LOW();
    IIC_Delay();
    SCL_HIGH();
    IIC_Delay();
    SDA_HIGH();
    IIC_Delay();
}

void IIC_Send_Byte(uint8_t byte) {
    SDA_OUT();
		SCL_LOW();
    for (uint8_t i = 0; i < 8; i++) {
        if ((byte<<i) & 0x80) {
            SDA_HIGH();
        } else {
            SDA_LOW();
        }
				IIC_Delay();
        SCL_HIGH();
        IIC_Delay();
        SCL_LOW();
        IIC_Delay();
    }
}

uint8_t IIC_Read_Byte(uint8_t ack) {
    uint8_t byte = 0;
    SDA_IN();
    for (uint8_t i = 0; i < 8; i++) {
        SCL_LOW();
        IIC_Delay();
        SCL_HIGH();
        byte <<= 1;
        if (READ_SDA()) {
            byte |= 0x01;
        }
        IIC_Delay();
    }
    return byte;
}

uint8_t IIC_Wait_Ack(void)
{
    uint8_t wait;
    SDA_IN();
		IIC_Delay();
		SCL_HIGH();
		IIC_Delay();
    while (READ_SDA())
    {
        wait++;
        if (wait > 200)
        {
            IIC_Stop();
            return 1;
        }
    }
    SCL_LOW();
    return 0;
}

void IIC_Ack(void) {
    SCL_LOW();
    SDA_OUT();
    SDA_LOW();
    IIC_Delay();
    SCL_HIGH();
    IIC_Delay();
    SCL_LOW();
}


void IIC_NAck(void) {
    SCL_LOW();
    SDA_OUT();
    SDA_HIGH();
    IIC_Delay();
    SCL_HIGH();
    IIC_Delay();
    SCL_LOW();
}


void Soft_IIC_Write_Command(uint8_t deviceAddr, uint8_t command) {
    IIC_Start();
    IIC_Send_Byte(deviceAddr << 1); // 发送设备地址和写位
    IIC_Wait_Ack();
    IIC_Send_Byte(command); // 发送命令
    IIC_Wait_Ack();
    IIC_Stop();
}

void SHT40_Start_Measurement(void) {
    Soft_IIC_Write_Command(SHT40_ADDRESS, SHT40_COMMAND_MEASURE_HIGH_PRECISION);
    delay_ms(10); // 根据数据手册说明，延时10ms
}

void SHT40_Read_Measurement(uint8_t* data, uint8_t length) {
    IIC_Start();
    IIC_Send_Byte((SHT40_ADDRESS << 1) | 0x01); // 发送设备地址和读位
    IIC_Wait_Ack();
    for (uint8_t i = 0; i < length-1; i++) {
      *data = IIC_Read_Byte(i < (length - 1)); // 读取数据并发送应答信号
			data++;
			IIC_Ack();
    }
		*data = IIC_Read_Byte(0);
		IIC_NAck();
    IIC_Stop();
}

void SHT40_Show_Temp_Humid(void)
{
	double Temperature = 0;
	double Humidity = 0;
	uint8_t readData[6]={0};
	int temp_num[3],humid_num[3];
	
	Lcd_Clear(BLACK);
	Gui_DrawFont_GBK16(48,0,LIGHTBLUE,BLACK,"温湿度计");
	Gui_DrawFont_Num32(80,24,ORANGE,BLACK,10);//绘制小数点
	Gui_DrawFont_Num32(80,64,ORANGE,BLACK,10);
	Gui_DrawFont_Num32(128,24,LIGHTBLUE,BLACK,13);//绘制单位
	Gui_DrawFont_Num32(128,64,LIGHTBLUE,BLACK,12);
	uint8_t cnt=200;
	while(key_status[2]==0)
	{
		cnt++;
		if(cnt>=200)
		{
			cnt = 0;
			SHT40_Start_Measurement();
			SHT40_Read_Measurement((uint8_t*)readData,6);
			Temperature = (1.0 * 175 * (readData[0] * 256 + readData[1])) / 65535.0 - 45;
			Humidity = (1.0 * 125 * (readData[3] * 256 + readData[4])) / 65535.0 - 6.0;
			int Temp = (int)(Temperature * 10);		//放大温湿度
			int Humid = (int)(Humidity * 10);
			for(uint8_t i = 0;i<3;i++)
			{
				temp_num[2-i]=(int)(Temp/(pow(10,i))) %10;
				humid_num[2-i]=(int)(Humid/(pow(10,i))) %10;
			}
			for(uint8_t i = 0;i<2;i++)
			{
				Gui_DrawFont_Num32(16+i*32,24,GREEN,BLACK,temp_num[i]);
				Gui_DrawFont_Num32(16+i*32,64,RED,BLACK,humid_num[i]);
			}
			Gui_DrawFont_Num32(96,24,GREEN,BLACK,temp_num[2]);
			Gui_DrawFont_Num32(96,64,RED,BLACK,humid_num[2]);
		}
		delay_ms(5);
	}
}
