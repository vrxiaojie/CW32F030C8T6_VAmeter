/*
 * ���������˺�		: 	VRС��      	https://space.bilibili.com/11526854
 * ��������Դ�㳡	: 	vrxiaojie   https://oshwhub.com/vrxiaojie/
 *
 *   Date           Author
 * 2024-08-10     VRxiaojie
 */

#include "main.h"

//��ʼ������״̬����
uint16_t key_pin[5] = {UP_KEY_PIN,DOWN_KEY_PIN,LEFT_KEY_PIN,RIGHT_KEY_PIN,CONFIRM_KEY_PIN};
GPIO_TypeDef* key_pin_port[5] = {UP_KEY_PORT,DOWN_KEY_PORT,LEFT_KEY_PORT,RIGHT_KEY_PORT,CONFIRM_KEY_PORT};
uint8_t key_status[5]={0,0,0,0,0};
uint8_t key_flag[5]={0,0,0,0,0};

//��ʼ��ESP8266״̬��ʶ������ESP8266�Ƿ񿪻����Ƿ����ӵ�CW32��
uint8_t ESP8266_Status = 0;
uint8_t ESP8266_Last_Status = 0;


/**************************************************************************************
	�������ƣ�	GPIOA_IRQHandler

	�������ܣ�	GPIOA�жϴ�����

	��ڲ�����	��

	���ز�����	��
**************************************************************************************/
void GPIOA_IRQHandler(void)
{
    if(CW_GPIOA->ISR_f.PIN10)

    {
        GPIOA_INTFLAG_CLR(bv10);//���CW_GPIO�жϱ�־
    }

}

/**************************************************************************************
	�������ƣ�	GPIO_User_Init

	�������ܣ�	��ʼ���û������GPIO�˿�

	��ڲ�����	��

	���ز�����	��
**************************************************************************************/
void GPIO_User_Init()
{
    GPIO_InitTypeDef	GPIO_InitStruct; // GPIO��ʼ���ṹ��

    __RCC_GPIOA_CLK_ENABLE();// ʹ��GPIOʱ��
    __RCC_GPIOB_CLK_ENABLE();
    __RCC_GPIOC_CLK_ENABLE();

    GPIO_InitStruct.Pins = ESP8266_EN_PIN;			// PB14��ΪESP8266����ͣ����
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;		//�������
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;		// ����ٶȸ�
    GPIO_Init(ESP8266_EN_PORT, &GPIO_InitStruct);			// ��ʼ��

    GPIO_WritePin(ESP8266_EN_PORT,ESP8266_EN_PIN,GPIO_Pin_RESET); //Ĭ�Ϲر�ESP8266

    GPIO_InitStruct.Pins = ESP8266_PIN;				// PB15��Ϊ���ESP8266״̬������
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;			//��������
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;		// ����ٶȸ�
    GPIO_Init(ESP8266_PORT, &GPIO_InitStruct);		// ��ʼ��
	
	GPIO_InitStruct.Pins = GPIO_PIN_13;				// PB13��ΪLDO����ͣ����
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;		//�������
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;		// ����ٶȸ�
    GPIO_Init(CW_GPIOB, &GPIO_InitStruct);			// ��ʼ��
	GPIO_WritePin(CW_GPIOB,GPIO_PIN_13,GPIO_Pin_SET);  //Ĭ�Ͽ���LDO
	
	GPIO_InitStruct.Pins = SCL_PIN|SDA_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	GPIO_Init(CW_GPIOA, &GPIO_InitStruct);
}


/**************************************************************************************
	�������ƣ�	main

	�������ܣ�	����������

	��ڲ�����	��

	���ز�����	��
**************************************************************************************/
int32_t main(void)
{
    board_init();	// �������ʼ��

    //��Ļ��ʼ��
    Lcd_Init();  //��ʼ��LCD��
    Lcd_Clear(BLACK);

    uart1_init(115200);	// ����1������115200
    GPIO_User_Init(); //GPIO��ʼ��
    BTIM1_Init();
    BTIM2_Init();
	GTIM3_PWM_Init();
    Key_Init();
    //�ϵ����ESP8266Ϊ�� Wi-Fi ģʽ���ر��Զ�����WIFI��ʡ�磩
    USART_SendString(CW_UART1,(uint8_t*)"AT+CWMODE=0,0\r\n");
    Show_Main_Menu();

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
		//���������Ӳ˵�
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
		
		//��ESP8266״̬�����仯
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
