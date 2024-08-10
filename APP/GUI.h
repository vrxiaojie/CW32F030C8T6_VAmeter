/*
 * 哔哩哔哩账号		: 	VR小杰      	https://space.bilibili.com/11526854
 * 嘉立创开源广场	: 	vrxiaojie   https://oshwhub.com/vrxiaojie/
 *
 *   Date           Author
 * 2024-08-10     VRxiaojie
 */
#ifndef GUI_H_
#define GUI_H_

	#include "board.h"

	void Gui_DrawLine(uint16_t x0, uint16_t y0,uint16_t x1, uint16_t y1,uint16_t Color);  
	void Gui_DrawFont_GBK16(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, char *s);
	void Gui_DrawFont_GBK24(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *s);
	void Gui_DrawFont_Num32(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint16_t num) ;
	void showimage_16(const unsigned char *p, uint8_t x1,uint8_t y1);
	void Show_Logo(uint8_t x1,uint8_t y1);
	void Gui_DrawFont_GBK24(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *s);
	void Gui_DrawFont_Num16(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint16_t num);

#endif
