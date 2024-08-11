/**
* @file         Lcd_Driver.c
* @brief		LCD��Ļ����Դ�ļ�
* @author		VRxiaojie
* @date		    2024-08-11
* @version	    v1.0
* @note 	 	��������	:		VRС��		https://space.bilibili.com/11526854 <br>
* 				��������Դ�㳡:	vrxiaojie	https://oshwhub.com/vrxiaojie/	
*/
/******************************************************************************
TFT��Ļ����
              GND   ��Դ��
              VCC   ��5V��3.3v��Դ
              SCL   ��PA4
              SDA   ��PA3
              RES   ��PA2
              DC    ��PA1
              CS    ��PA0
			  BL	��PC15
*******************************************************************************/
#include "board.h"
#include "Lcd_Driver.h"

uint8_t lcd_brightness;   ///<LCD��Ļ����

/**
* @brief		��ʼ��LCD��Ļ��GPIO����
* @param		��
* @return		��	
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
* @brief		��SPI���ߴ���һ��8λ������
* @param		Data ����
* @return		��	
*/
void  SPI_WriteData(uint8_t Data)
{
    unsigned char i=0;
    for(i=8; i>0; i--)
    {
        if(Data&0x80)
            LCD_SDA_SET; //�������
        else LCD_SDA_CLR;

        LCD_SCL_CLR;
        LCD_SCL_SET;
        Data<<=1;
    }
}

/**
* @brief		��Һ����дһ��8λָ��
* @param		Index ��ַ
* @return		��	
*/
void Lcd_WriteIndex(uint8_t Index)
{
    //SPI д����ʱ��ʼ
    LCD_CS_CLR;
    LCD_RS_CLR;
    SPI_WriteData(Index);
    LCD_CS_SET;
}

/**
* @brief		��Һ����дһ��8λ����
* @param		Data ����
* @return		��	
*/

void Lcd_WriteData(uint8_t Data)
{
    LCD_CS_CLR;
    LCD_RS_SET;
    SPI_WriteData(Data);
    LCD_CS_SET;
}

/**
* @brief		��SPI���ߴ���һ��16λ����
* @param		Data ����
* @return		��	
*/

void LCD_WriteData_16Bit(uint16_t Data)
{
    LCD_CS_CLR;
    LCD_RS_SET;
    SPI_WriteData(Data>>8); 	//д���8λ����
    SPI_WriteData(Data); 			//д���8λ����
    LCD_CS_SET;
}

/**
* @brief		��Һ�����Ĵ���д����
* @param		Index �Ĵ�����ַ
* @param		Data ����
* @return		��	
*/

void Lcd_WriteReg(uint8_t Index,uint8_t Data)
{
    Lcd_WriteIndex(Index);
    Lcd_WriteData(Data);
}

/**
* @brief		����LCD��Ļ
* @param		��
* @return		��	
*/
void Lcd_Reset(void)
{
    LCD_RST_CLR;
    delay_ms(100);
    LCD_RST_SET;
    delay_ms(50);
}

/**
* @brief		LCD��Ļ��ʼ��
* @param		��
* @return		��	
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
* @brief		����lcd��ʾ�����ڴ�����д�������Զ�����
* @param		x_start ��ʾ��������x����
* @param		y_start ��ʾ��������y����
* @param		x_end ��ʾ�����յ��x����
* @param		y_end ��ʾ�����յ��y����
* @return		��	
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
* @brief		����lcd��ʾ��ʼ��
* @param		x �õ�ĺ�����
* @param		y �õ������
* @return		��	
*/

void Lcd_SetXY(uint16_t x,uint16_t y)
{
    Lcd_SetRegion(x,y,x,y);
}


/**
* @brief		��һ����
* @param		x �õ�ĺ�����
* @param		y �õ������
* @param		Data ��ɫֵ
* @return		��	
*/

void Gui_DrawPoint(uint16_t x,uint16_t y,uint16_t Data)
{
    Lcd_SetRegion(x,y,x+1,y+1);
    LCD_WriteData_16Bit(Data);

}

/**
* @brief		��TFTĳһ�����ɫ
* @param		x �õ�ĺ�����
* @param		y �õ������
* @param		Data ��ɫֵ
* @return		color �õ���ɫֵ
*/

uint32_t Lcd_ReadPoint(uint16_t x,uint16_t y)
{
    unsigned int Data;
    Lcd_SetXY(x,y);

    //Lcd_ReadData();//���������ֽ�
    //Data=Lcd_ReadData();
    Lcd_WriteData(Data);
    return Data;
}

/**
* @brief		ȫ����������
* @param		color ������ɫ
* @return		��
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
* @brief		����������������
* @param		Color ������ɫ
* @param		x1 ��ʾ��������x����
* @param		y1 ��ʾ��������y����
* @param		x2 ��ʾ�����յ��x����
* @param		y2 ��ʾ�����յ��y����
* @return		��
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
* @brief		����LCD��Ļ������
* @param		light ����ֵ 0-100
* @return		��
* @note			ʹ��PWM����
*/
void Set_Lcd_Brightness(uint8_t light)	
{
	GTIM_SetCompare2(CW_GTIM3, 100-light);
}
