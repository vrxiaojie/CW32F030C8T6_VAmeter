/**
* @file         GUI.c
* @brief		GUI�������Դ�ļ�
* @author		VRxiaojie
* @date		    2024-08-11
* @version	    v1.0
* @note 	 	��������	:		VRС��		https://space.bilibili.com/11526854 <br>
* 				��������Դ�㳡:	vrxiaojie	https://oshwhub.com/vrxiaojie/	
*/
 
#include "GUI.h"
#include "Lcd_Driver.h"
#include "Font.h"
#include "Picture.h"

/**
* @brief		���ߺ�����ʹ��Bresenham �����㷨
* @param		x0 ��һ����ĺ�����
* @param		y0 ��һ�����������
* @param		x1 �ڶ�����ĺ�����
* @param		y1 �ڶ������������
* @return		��
*/
void Gui_DrawLine(uint16_t x0, uint16_t y0,uint16_t x1, uint16_t y1,uint16_t Color)   
{
int dx,             // difference in x's
    dy,             // difference in y's
    dx2,            // dx,dy * 2
    dy2, 
    x_inc,          // amount in pixel space to move during drawing
    y_inc,          // amount in pixel space to move during drawing
    error,          // the discriminant i.e. error i.e. decision variable
    index;          // used for looping	


	Lcd_SetXY(x0,y0);
	dx = x1-x0;//����x����
	dy = y1-y0;//����y����

	if (dx>=0)
	{
		x_inc = 1;
	}
	else
	{
		x_inc = -1;
		dx    = -dx;  
	} 
	
	if (dy>=0)
	{
		y_inc = 1;
	} 
	else
	{
		y_inc = -1;
		dy    = -dy; 
	} 

	dx2 = dx << 1;
	dy2 = dy << 1;

	if (dx > dy)//x�������y���룬��ôÿ��x����ֻ��һ���㣬ÿ��y���������ɸ���
	{//���ߵĵ�������x���룬��x���������
		// initialize error term
		error = dy2 - dx; 

		// draw the line
		for (index=0; index <= dx; index++)//Ҫ���ĵ������ᳬ��x����
		{
			//����
			Gui_DrawPoint(x0,y0,Color);
			
			// test if error has overflowed
			if (error >= 0) //�Ƿ���Ҫ����y����ֵ
			{
				error-=dx2;

				// move to next line
				y0+=y_inc;//����y����ֵ
			} // end if error overflowed

			// adjust the error term
			error+=dy2;

			// move to the next pixel
			x0+=x_inc;//x����ֵÿ�λ���󶼵���1
		} // end for
	} // end if |slope| <= 1
	else//y�����x�ᣬ��ÿ��y����ֻ��һ���㣬x�����ɸ���
	{//��y��Ϊ��������
		// initialize error term
		error = dx2 - dy; 

		// draw the line
		for (index=0; index <= dy; index++)
		{
			// set the pixel
			Gui_DrawPoint(x0,y0,Color);

			// test if error overflowed
			if (error >= 0)
			{
				error-=dy2;

				// move to next line
				x0+=x_inc;
			} // end if error overflowed

			// adjust the error term
			error+=dx2;

			// move to the next pixel
			y0+=y_inc;
		} // end for
	} // end else |slope| > 1
}



/**
* @brief		����GBK�����16���ء���8���ص�Ӣ�ġ��ַ������֣����ֵĸߡ����Ϊ16����
* @param		x ���ֵ����Ͻ����صĺ�����
* @param		y ���ֵ����Ͻ����ص�������
* @param		fc �ֵ���ɫ
* @param		bc �ֵı�����ɫ
* @param		s Ҫ���Ƶ��ַ�
* @return		��
*/
void Gui_DrawFont_GBK16(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, char *s)
{
	unsigned char i,j;
	unsigned short k,x0;
	x0=x;
	while(*s) 
	{	
		if((*s) < 128) 
		{
			k=*s;
			if (k==13) 
			{
				x=x0;
				y+=16;
			}
			else 
			{
				if (k>32) k-=32; else k=0;
	
			    for(i=0;i<16;i++)
				for(j=0;j<8;j++) 
					{
				    	if(asc16[k*16+i]&(0x80>>j))	Gui_DrawPoint(x+j,y+i,fc);
						else 
						{
							if (fc!=bc) Gui_DrawPoint(x+j,y+i,bc);
						}
					}
				x+=8;
			}
			s++;
		}
			
		else 
		{
		

			for (k=0;k<hz16_num;k++) 
			{
			  if ((hz16[k].Index[0]==*(s))&&(hz16[k].Index[1]==*(s+1)))
			  { 
				    for(i=0;i<16;i++)
				    {
						for(j=0;j<8;j++) 
							{
						    	if(hz16[k].Msk[i*2]&(0x80>>j))	Gui_DrawPoint(x+j,y+i,fc);
								else {
									if (fc!=bc) Gui_DrawPoint(x+j,y+i,bc);
								}
							}
						for(j=0;j<8;j++) 
							{
						    	if(hz16[k].Msk[i*2+1]&(0x80>>j))	Gui_DrawPoint(x+j+8,y+i,fc);
								else 
								{
									if (fc!=bc) Gui_DrawPoint(x+j+8,y+i,bc);
								}
							}
				    }
				}
			  }
			s+=2;x+=16;
		} 
		
	}
}

/**
* @brief		���Ƹ�16���ء���8���ص�����
* @param		x �����ֵ����Ͻ����صĺ�����
* @param		y �����ֵ����Ͻ����ص�������
* @param		fc ���ֵ���ɫ
* @param		bc ���ֵı�����ɫ
* @param		num Ҫ���Ƶ�����
* @return		��
*/
void Gui_DrawFont_Num16(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint16_t num)
{
	unsigned char i,j;
	 for(i=0;i<16;i++)
		for(j=0;j<8;j++) 
			{
				if(asc16[(16+num)*16+i]&(0x80>>j))	Gui_DrawPoint(x+j,y+i,fc);
				else 
				{
					if (fc!=bc) Gui_DrawPoint(x+j,y+i,bc);
				}
			}
}



/**
* @brief		���Ƴ���Ϊ32���ص��������������
* @param		x �����ֵ����Ͻ����صĺ�����
* @param		y �����ֵ����Ͻ����ص�������
* @param		fc ���ֵ���ɫ
* @param		bc ���ֵı�����ɫ
* @param		num Ҫ���Ƶ�����
* @return		��
* @note			�ڵ�ѹ����������ģʽ�Լ���ʪ�ȼƹ����У���ʾ��ֵ��
*/
void Gui_DrawFont_Num32(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint16_t num)
{
	unsigned char i,j,k,c;
    for(i=0;i<32;i++)
	{
		for(j=0;j<4;j++) 
		{
			c=*(sz32+num*32*4+i*4+j);
			for (k=0;k<8;k++)	
			{
	
		    	if(c&(0x80>>k))	Gui_DrawPoint(x+j*8+k,y+i,fc);
				else {
					if (fc!=bc) Gui_DrawPoint(x+j*8+k,y+i,bc);
				}
			}
		}
	}
}


/**
* @brief		����16*16���ص�ͼƬ
* @param		x ��ͼƬ���Ͻ����صĺ�����
* @param		y ��ͼƬ���Ͻ����ص�������
* @param		p ͼƬ������Picture.h�ж���
* @return		��
* @note			���ڻ���״̬��ͼ��
*/
void showimage_16(const unsigned char *p, uint8_t x1,uint8_t y1) //��ʾ16*16ͼƬ
{	
	uint16_t i;
	unsigned char picH,picL;
	Lcd_SetRegion(x1,y1,x1+13,y1+15);		//��������
		for(i=0;i<16*16;i++)
	 {	
		picL=*(p+i*2);	//���ݵ�λ��ǰ
		picH=*(p+i*2+1);				
		LCD_WriteData_16Bit(picH<<8|picL);  						
		}		
}



/**
* @brief		����ϵͳLOGO
* @param		x LOGO�����Ͻ����صĺ�����
* @param		y LOGO�����Ͻ����ص�������
* @return		��
* @note			LOGO�ֱ���Ϊ100*32
*/
void Show_Logo(uint8_t x1,uint8_t y1)
{
	const unsigned char *p = logo;
	uint16_t i;
	unsigned char picH,picL;
	Lcd_SetRegion(x1,y1,x1+97,y1+31);		//��������
		for(i=0;i<100*32;i++)
	 {	
		picL=*(p+i*2);	//���ݵ�λ��ǰ
		picH=*(p+i*2+1);				
		LCD_WriteData_16Bit(picH<<8|picL);  				
}
}

