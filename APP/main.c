/**
* @file         main.c
* @brief		������
* @author		VRxiaojie
* @date		    2024-08-11
* @version	    v1.0
* @note 		��������	:		VRС��		https://space.bilibili.com/11526854 <br>
* 				��������Դ�㳡:	vrxiaojie	https://oshwhub.com/vrxiaojie/	
*/


#include "main.h"

/**
* @brief		����������
* @param		��
* @return		��
* @note			��һЩ��ʼ���Լ����˵��Ĳ����ж��߼�
*/
int32_t main(void)
{
    board_init();	//�������ʼ��
    Lcd_Init();  //��ʼ��LCD��
    Lcd_Clear(BLACK); //��Ļ����ɫ��Ϊ��

    uart1_init(115200);	// ����1������115200
    GPIO_User_Init(); //�û������GPIO��ʼ��
    BTIM1_Init();	//������ʱ��1��ʼ��
    BTIM2_Init();	//������ʱ��2��ʼ��
	GTIM3_PWM_Init();	//ͨ�ö�ʱ��3��ʼ��
    Key_Init();		//������ʼ��
    //�ϵ����ESP8266Ϊ�� Wi-Fi ģʽ���ر��Զ�����WIFI��ʡ�磩
    USART_SendString(CW_UART1,(uint8_t*)"AT+CWMODE=0,0\r\n");
    Show_Main_Menu();	//��ʾ���˵�

    while(1)
    {	
		uint8_t btn;
		for(btn = 0;btn<5;btn++)
		{
			if(key_flag[btn]==1)
				break;
		}
		if(btn<5)
			key_flag[btn]=0;
		
		//�����������˵�
		if(in_sub_menu_flag == 0&& in_progress_flag ==0)
		{
			switch(btn)
		{
			//����
			case 0:
				Restore_Main_Menu_Button(main_menu_index);
				switch(main_menu_index)
				{
					case 0:
						main_menu_index = MAIN_MENU_INDEX;
						break;
					default:
						main_menu_index --;
						break;
				}
				sub_menu_index = 0;
				Choose_Main_Function();
				Show_Sub_Menu();
				break;
			//����
			case 1:
				Restore_Main_Menu_Button(main_menu_index);
				main_menu_index = (main_menu_index+1)%(MAIN_MENU_INDEX+1);
				sub_menu_index = 0;
				Choose_Main_Function();
				Show_Sub_Menu();
				break;
			//����
			case 3:
				in_sub_menu_flag = 1;
				Choose_Sub_Function();
				break;
		}
		}
		//�������ڴμ��˵�
		else if(in_sub_menu_flag == 1 && in_progress_flag ==0)
		{
			switch(btn){
			//����
			case 0:
				Restore_Sub_Menu_Button(sub_menu_index);
				switch(sub_menu_index)
				{
					case 0:
						sub_menu_index = sub_menu_maxidx[main_menu_index];
						break;
					default:
						sub_menu_index --;
						break;
				}
				Choose_Sub_Function();
				break;
			//����
			case 1:
				Restore_Sub_Menu_Button(sub_menu_index);
				sub_menu_index = (sub_menu_index+1)%(sub_menu_maxidx[main_menu_index]+1);
				Choose_Sub_Function();
				break;
			//����
			case 2:
				in_sub_menu_flag = 0;
				Restore_Sub_Menu_Button(sub_menu_index);
				break;
			case 4:
				Run_Function();
				break;
			}
		}
		
		//��������������˵�
		if(in_progress_flag == 1 && btn == 2)
		{
			Lcd_Clear(BLACK);
			Show_Main_Menu();
			in_progress_flag = 0;
		}
		
		//��ESP8266״̬�����仯��ִ�в���
		if(ESP8266_Last_Status!=ESP8266_Status && in_progress_flag == 0)
		{
			ESP8266_Last_Status=ESP8266_Status;
			if(ESP8266_Status==1)
				ESP8266_Reset(0);
			else
			{
				FUN[2][0] = Connect_WIFI;
				FUN[3][0] = MQTT_Setup;
				sub_menu_button[2][0]="����WIFI  ";
				sub_menu_button[3][0]="����MQTT  ";
				mqtt_status = 0;
				WIFI_Status = 0;
			}
			Show_Main_Menu();
		}
		
		if(bat_cnt>=30)//ÿ30sˢ��һ�ε�ص���
		{
			Show_Status_Bar();
			bat_cnt=0;
		}
    }
}
