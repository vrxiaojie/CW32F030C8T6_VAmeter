/**
* @file         Lcd_Driver.h
* @brief		LCD屏幕驱动头文件
* @author		VRxiaojie
* @date		    2024-08-11
* @version	    v1.0
* @note 	 	哔哩哔哩	:		VR小杰		https://space.bilibili.com/11526854 <br>
* 				嘉立创开源广场:	vrxiaojie	https://oshwhub.com/vrxiaojie/	
*/
 
#ifndef LCD_DRIVER_H_
#define LCD_DRIVER_H_

	#include "board.h"

	#define WHITE         	 0xFFFF
	#define BLACK         	 0x0000
	#define ORANGE 			 0XFD20
	#define BLUE           	 0x001F  
	#define BRED             0XF81F
	#define GRED 			 0XFFE0
	#define GBLUE			 0X07FF
	#define RED           	 0xF800
	#define MAGENTA       	 0xF81F
	#define GREEN         	 0x07E0
	#define CYAN          	 0x7FFF
	#define YELLOW        	 0xFFE0
	#define PURPLE           0x780F //紫色  
	#define BROWN 			 0XBC40 //棕色
	#define BRRED 			 0XFC07 //棕红色
	#define GRAY  			 0X8430 //灰色
	#define DARKBLUE      	 0x4C1C	//深蓝色
	#define LIGHTBLUE      	 0x67A	//浅蓝色  
	#define GRAYBLUE       	 0X5458 //灰蓝色
	#define LIGHTGREEN     	 0X841F //浅绿色
	#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色
	#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
	#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)





	#define LCD_CTRLA   	  	CW_GPIOA		//定义TFT数据端口
	#define LCD_CTRLC   	  	CW_GPIOC		//定义TFT数据端口



	#define LCD_SCL        	GPIO_PIN_4	//PA4--->>TFT --SCL/SCK
	#define LCD_SDA        	GPIO_PIN_3	//PA3 MOSI--->>TFT --SDA/DIN
	#define LCD_CS        	GPIO_PIN_0  //PA0--->>TFT --CS/CE

	#define LCD_LED        	GPIO_PIN_15  //PC15--->>TFT --BL
	#define LCD_RS         	GPIO_PIN_1	//PA1--->>TFT --RS/DC
	#define LCD_RST     	GPIO_PIN_2	//PA2--->>TFT --RST

	//#define LCD_CS_SET(x) LCD_CTRL->ODR=(LCD_CTRL->ODR&~LCD_CS)|(x ? LCD_CS:0)

	//液晶控制口置1操作语句宏定义
	#define	LCD_SCL_SET  	LCD_CTRLA->BSRR=LCD_SCL    
	#define	LCD_SDA_SET  	LCD_CTRLA->BSRR=LCD_SDA   
	#define	LCD_CS_SET  	LCD_CTRLA->BSRR=LCD_CS  

		
	#define	LCD_LED_SET  	LCD_CTRLC->BSRR=LCD_LED   
	#define	LCD_RS_SET  	LCD_CTRLA->BSRR=LCD_RS 
	#define	LCD_RST_SET  	LCD_CTRLA->BSRR=LCD_RST 
	//液晶控制口置0操作语句宏定义
	#define	LCD_SCL_CLR  	LCD_CTRLA->BRR=LCD_SCL  
	#define	LCD_SDA_CLR  	LCD_CTRLA->BRR=LCD_SDA 
	#define	LCD_CS_CLR  	LCD_CTRLA->BRR=LCD_CS 
		
	#define	LCD_LED_CLR  	LCD_CTRLC->BRR=LCD_LED 
	#define	LCD_RST_CLR  	LCD_CTRLA->BRR=LCD_RST
	#define	LCD_RS_CLR  	LCD_CTRLA->BRR=LCD_RS 

	#define LCD_DATAOUT(x) LCD_DATA->ODR=x; //数据输出
	#define LCD_DATAIN     LCD_DATA->IDR;   //数据输入

	#define LCD_WR_DATA(data){\
	LCD_RS_SET;\
	LCD_CS_CLR;\
	LCD_DATAOUT(data);\
	LCD_WR_CLR;\
	LCD_WR_SET;\
	LCD_CS_SET;\
	} 
	
	//竖屏
	//#define X_MAX_PIXEL	        128
	//#define Y_MAX_PIXEL	        160

	//横屏
	#define X_MAX_PIXEL	        160
	#define Y_MAX_PIXEL	        128



	void LCD_GPIO_Init(void);
	void Lcd_WriteIndex(uint8_t Index);
	void Lcd_WriteData(uint8_t Data);
	void Lcd_WriteReg(uint8_t Index,uint8_t Data);
	uint16_t Lcd_ReadReg(uint8_t LCD_Reg);
	void Lcd_Reset(void);
	void Lcd_Init(void);
	void Lcd_Clear(uint16_t Color);
	void Lcd_SetXY(uint16_t x,uint16_t y);
	void Gui_DrawPoint(uint16_t x,uint16_t y,uint16_t Data);
	uint32_t Lcd_ReadPoint(uint16_t x,uint16_t y);
	void Lcd_SetRegion(uint16_t x_start,uint16_t y_start,uint16_t x_end,uint16_t y_end);
	void LCD_WriteData_16Bit(uint16_t Data);
	void Lcd_Part_Clear(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint16_t Color);
	void Set_Lcd_Brightness(uint8_t light);
	extern uint8_t lcd_brightness;	

#endif
