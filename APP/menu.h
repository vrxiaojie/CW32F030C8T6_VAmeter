<<<<<<< HEAD
/**
* @file         menu.h
* @brief		�˵���غ���ͷ�ļ�
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
#ifndef MENU_H_
#define MENU_H_

	#include "board.h"
	#include "Picture.h"

<<<<<<< HEAD
	#define MAIN_MENU_INDEX 4  ///<���˵���������±꣬��0��ʼ
	
	#define SUB_MENU_0_INDEX 1 ///<��һ���μ��˵������������ͬ
=======
	#define MAIN_MENU_INDEX 4  //���˵���������±꣬��0��ʼ
	//�����Ӳ˵��������
	#define SUB_MENU_0_INDEX 1
>>>>>>> 0dfd7a7e8c0db99ec07f30fe2edd2e84eaf20952
	#define SUB_MENU_1_INDEX 0
	#define SUB_MENU_2_INDEX 1
	#define SUB_MENU_3_INDEX 0
	#define SUB_MENU_4_INDEX 4


	extern uint8_t in_progress_flag;
<<<<<<< HEAD
	extern uint8_t in_sub_menu_flag;
=======
>>>>>>> 0dfd7a7e8c0db99ec07f30fe2edd2e84eaf20952
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
