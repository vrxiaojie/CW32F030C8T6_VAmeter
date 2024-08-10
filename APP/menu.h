/*
 * 哔哩哔哩账号		: 	VR小杰      	https://space.bilibili.com/11526854
 * 嘉立创开源广场	: 	vrxiaojie   https://oshwhub.com/vrxiaojie/
 *
 *   Date           Author
 * 2024-08-10     VRxiaojie
 */
#ifndef MENU_H_
#define MENU_H_

	#include "board.h"
	#include "Picture.h"

	#define MAIN_MENU_INDEX 4  //主菜单最大索引下标，从0开始
	//各级子菜单最大索引
	#define SUB_MENU_0_INDEX 1
	#define SUB_MENU_1_INDEX 0
	#define SUB_MENU_2_INDEX 1
	#define SUB_MENU_3_INDEX 0
	#define SUB_MENU_4_INDEX 4


	extern uint8_t in_progress_flag;
	extern uint8_t main_menu_index,sub_menu_index;
	extern char* main_menu_button[];
	extern char* sub_menu_button[5][5];
	extern uint8_t sub_menu_maxidx[5];
	extern void (*FUN[5][5])();

	void Show_Main_Menu(void);
	void Choose_Main_Function(void);
	void Choose_Sub_Function(void);
	void Restore_Main_Menu_Button(uint8_t idx);
	void Restore_Sub_Menu_Button(uint8_t idx);
	void Run_Function(void);
	void Show_Status_Bar(void);
	void Show_Sub_Menu(void);

#endif
