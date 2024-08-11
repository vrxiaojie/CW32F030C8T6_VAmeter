/**
* @file         Lcd_Driver.c
* @brief		LCD屏幕驱动源文件
* @author		VRxiaojie
* @date		    2024-08-11
* @version	    v1.0
* @note 	 	哔哩哔哩	:		VR小杰		https://space.bilibili.com/11526854 <br>
* 				嘉立创开源广场:	vrxiaojie	https://oshwhub.com/vrxiaojie/	
*/
/******************************************************************************
TFT屏幕接线
              GND   电源地
              VCC   接5V或3.3v电源
              SCL   接PA4
              SDA   接PA3
              RES   接PA2
              DC    接PA1
              CS    接PA0
			  BL	接PC15
*******************************************************************************/
#include "board.h"
#include "Lcd_Driver.h"

uint8_t lcd_brightness;   ///<LCD屏幕亮度

/**
* @brief		初始化LCD屏幕的GPIO配置
* @param		无
* @return		无	
*/
void LCD_GPIO_Init(void)
{

    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHBPeriphClk_Enable( RCC_AHB_PERIPH_GPIOA,ENABLE);

    GPIO_InitStructure.Pins = GPIO_PIN_0| GPIO_PIN_1| GPIO_PIN_2| GPIO_PIN_3| GPIO_PIN_4;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(CW_GPIOA, &GPIO_InitStructure);


    RCC_AHBPeriphClk_Enable( RCC_AHB_PERIPH_GPIOC,ENABLE);

    GPIO_InitStructure.Pins = GPIO_PIN_15;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(CW_GPIOC, &GPIO_InitStructure);
}

/**
* @brief		向SPI总线传输一个8位数数据
* @param		Data 数据
* @return		无	
*/
void  SPI_WriteData(uint8_t Data)
{
    unsigned char i=0;
    for(i=8; i>0; i--)
    {
        if(Data&0x80)
            LCD_SDA_SET; //输出数据
        else LCD_SDA_CLR;

        LCD_SCL_CLR;
        LCD_SCL_SET;
        Data<<=1;
    }
}

/**
* @brief		向液晶屏写一个8位指令
* @param		Index 地址
* @return		无	
*/
void Lcd_WriteIndex(uint8_t Index)
{
    //SPI 写命令时序开始
    LCD_CS_CLR;
    LCD_RS_CLR;
    SPI_WriteData(Index);
    LCD_CS_SET;
}

/**
* @brief		向液晶屏写一个8位数据
* @param		Data 数据
* @return		无	
*/

void Lcd_WriteData(uint8_t Data)
{
    LCD_CS_CLR;
    LCD_RS_SET;
    SPI_WriteData(Data);
    LCD_CS_SET;
}

/**
* @brief		向SPI总线传输一个16位数据
* @param		Data 数据
* @return		无	
*/

void LCD_WriteData_16Bit(uint16_t Data)
{
    LCD_CS_CLR;
    LCD_RS_SET;
    SPI_WriteData(Data>>8); 	//写入高8位数据
    SPI_WriteData(Data); 			//写入低8位数据
    LCD_CS_SET;
}

/**
* @brief		向液晶屏寄存器写数据
* @param		Index 寄存器地址
* @param		Data 数据
* @return		无	
*/

void Lcd_WriteReg(uint8_t Index,uint8_t Data)
{
    Lcd_WriteIndex(Index);
    Lcd_WriteData(Data);
}

/**
* @brief		重置LCD屏幕
* @param		无
* @return		无	
*/
void Lcd_Reset(void)
{
    LCD_RST_CLR;
    delay_ms(100);
    LCD_RST_SET;
    delay_ms(50);
}

/**
* @brief		LCD屏幕初始化
* @param		无
* @return		无	
*/

void Lcd_Init(void)
{
    LCD_GPIO_Init();
    Lcd_Reset(); //Reset before LCD Init.

    //LCD Init For 1.44Inch LCD Panel with ST7735R.
    Lcd_WriteIndex(0x11);//Sleep exit
    delay_ms (120);

    //ST7735R Frame Rate
    Lcd_WriteIndex(0xB1);
    Lcd_WriteData(0x01);
    Lcd_WriteData(0x2C);
    Lcd_WriteData(0x2D);

    Lcd_WriteIndex(0xB2);
    Lcd_WriteData(0x01);
    Lcd_WriteData(0x2C);
    Lcd_WriteData(0x2D);

    Lcd_WriteIndex(0xB3);
    Lcd_WriteData(0x01);
    Lcd_WriteData(0x2C);
    Lcd_WriteData(0x2D);
    Lcd_WriteData(0x01);
    Lcd_WriteData(0x2C);
    Lcd_WriteData(0x2D);

    Lcd_WriteIndex(0xB4); //Column inversion
    Lcd_WriteData(0x07);

    //ST7735R Power Sequence
    Lcd_WriteIndex(0xC0);
    Lcd_WriteData(0xA2);
    Lcd_WriteData(0x02);
    Lcd_WriteData(0x84);
    Lcd_WriteIndex(0xC1);
    Lcd_WriteData(0xC5);

    Lcd_WriteIndex(0xC2);
    Lcd_WriteData(0x0A);
    Lcd_WriteData(0x00);

    Lcd_WriteIndex(0xC3);
    Lcd_WriteData(0x8A);
    Lcd_WriteData(0x2A);
    Lcd_WriteIndex(0xC4);
    Lcd_WriteData(0x8A);
    Lcd_WriteData(0xEE);

    Lcd_WriteIndex(0xC5); //VCOM
    Lcd_WriteData(0x0E);

    Lcd_WriteIndex(0x36); //MX, MY, RGB mode
    Lcd_WriteData(0xA0);

    //ST7735R Gamma Sequence
    Lcd_WriteIndex(0xe0);
    Lcd_WriteData(0x0f);
    Lcd_WriteData(0x1a);
    Lcd_WriteData(0x0f);
    Lcd_WriteData(0x18);
    Lcd_WriteData(0x2f);
    Lcd_WriteData(0x28);
    Lcd_WriteData(0x20);
    Lcd_WriteData(0x22);
    Lcd_WriteData(0x1f);
    Lcd_WriteData(0x1b);
    Lcd_WriteData(0x23);
    Lcd_WriteData(0x37);
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x07);
    Lcd_WriteData(0x02);
    Lcd_WriteData(0x10);

    Lcd_WriteIndex(0xe1);
    Lcd_WriteData(0x0f);
    Lcd_WriteData(0x1b);
    Lcd_WriteData(0x0f);
    Lcd_WriteData(0x17);
    Lcd_WriteData(0x33);
    Lcd_WriteData(0x2c);
    Lcd_WriteData(0x29);
    Lcd_WriteData(0x2e);
    Lcd_WriteData(0x30);
    Lcd_WriteData(0x30);
    Lcd_WriteData(0x39);
    Lcd_WriteData(0x3f);
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x07);
    Lcd_WriteData(0x03);
    Lcd_WriteData(0x10);

    Lcd_WriteIndex(0x2a);
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x7f);

    Lcd_WriteIndex(0x2b);
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x9f);

    Lcd_WriteIndex(0xF0); //Enable test command
    Lcd_WriteData(0x01);
    Lcd_WriteIndex(0xF6); //Disable ram power save mode
    Lcd_WriteData(0x00);

    Lcd_WriteIndex(0x3A); //65k mode
    Lcd_WriteData(0x05);
    Lcd_WriteIndex(0x29);//Display on
	
	lcd_brightness = 50;
}


/**
* @brief		设置lcd显示区域，在此区域写点数据自动换行
* @param		x_start 显示区域起点的x坐标
* @param		y_start 显示区域起点的y坐标
* @param		x_end 显示区域终点的x坐标
* @param		y_end 显示区域终点的y坐标
* @return		无	
*/

void Lcd_SetRegion(uint16_t x_start,uint16_t y_start,uint16_t x_end,uint16_t y_end)
{
    Lcd_WriteIndex(0x2a);
    Lcd_WriteData(0x00);
    Lcd_WriteData(x_start);//Lcd_WriteData(x_start+2);
    Lcd_WriteData(0x00);
    Lcd_WriteData(x_end+2);

    Lcd_WriteIndex(0x2b);
    Lcd_WriteData(0x00);
    Lcd_WriteData(y_start+0);
    Lcd_WriteData(0x00);
    Lcd_WriteData(y_end+1);

    Lcd_WriteIndex(0x2c);

}


/**
* @brief		设置lcd显示起始点
* @param		x 该点的横坐标
* @param		y 该点的坐标
* @return		无	
*/

void Lcd_SetXY(uint16_t x,uint16_t y)
{
    Lcd_SetRegion(x,y,x,y);
}


/**
* @brief		画一个点
* @param		x 该点的横坐标
* @param		y 该点的坐标
* @param		Data 颜色值
* @return		无	
*/

void Gui_DrawPoint(uint16_t x,uint16_t y,uint16_t Data)
{
    Lcd_SetRegion(x,y,x+1,y+1);
    LCD_WriteData_16Bit(Data);

}

/**
* @brief		读TFT某一点的颜色
* @param		x 该点的横坐标
* @param		y 该点的坐标
* @param		Data 颜色值
* @return		color 该点颜色值
*/

uint32_t Lcd_ReadPoint(uint16_t x,uint16_t y)
{
    unsigned int Data;
    Lcd_SetXY(x,y);

    //Lcd_ReadData();//丢掉无用字节
    //Data=Lcd_ReadData();
    Lcd_WriteData(Data);
    return Data;
}

/**
* @brief		全屏清屏函数
* @param		color 填充的颜色
* @return		无
*/

void Lcd_Clear(uint16_t Color)
{
    unsigned int i,m;
    Lcd_SetRegion(0,0,X_MAX_PIXEL-1,Y_MAX_PIXEL-1);
    Lcd_WriteIndex(0x2C);
    for(i=0; i<X_MAX_PIXEL; i++)
        for(m=0; m<Y_MAX_PIXEL; m++)
        {
            LCD_WriteData_16Bit(Color);
        }
}

/**
* @brief		部分区域清屏函数
* @param		Color 填充的颜色
* @param		x1 显示区域起点的x坐标
* @param		y1 显示区域起点的y坐标
* @param		x2 显示区域终点的x坐标
* @param		y2 显示区域终点的y坐标
* @return		无
*/

void Lcd_Part_Clear(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint16_t Color)
{
	uint8_t i,j;
	Lcd_SetRegion(x1,y1,x2-1,y2-1);
	Lcd_WriteIndex(0x2C);
    for(i=0; i<x2-x1+1; i++)
        for(j=0; j<y2-y1+1; j++)
        {
            LCD_WriteData_16Bit(Color);
        }
}

/**
* @brief		设置LCD屏幕的亮度
* @param		light 亮度值 0-100
* @return		无
* @note			使用PWM调光
*/
void Set_Lcd_Brightness(uint8_t light)	
{
	GTIM_SetCompare2(CW_GTIM3, 100-light);
}
