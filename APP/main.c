/*
 * 哔哩哔哩账号		: 	VR小杰      	https://space.bilibili.com/11526854
 * 嘉立创开源广场	: 	vrxiaojie   https://oshwhub.com/vrxiaojie/
 *
 *   Date           Author
 * 2024-08-10     VRxiaojie
 */

#include "main.h"

//初始化按键状态变量
uint16_t key_pin[5] = {UP_KEY_PIN,DOWN_KEY_PIN,LEFT_KEY_PIN,RIGHT_KEY_PIN,CONFIRM_KEY_PIN};
GPIO_TypeDef* key_pin_port[5] = {UP_KEY_PORT,DOWN_KEY_PORT,LEFT_KEY_PORT,RIGHT_KEY_PORT,CONFIRM_KEY_PORT};
uint8_t key_status[5]={0,0,0,0,0};
uint8_t key_flag[5]={0,0,0,0,0};

//初始化ESP8266状态标识变量（ESP8266是否开机、是否连接到CW32）
uint8_t ESP8266_Status = 0;
uint8_t ESP8266_Last_Status = 0;


/**************************************************************************************
	函数名称：	GPIOA_IRQHandler

	函数功能：	GPIOA中断处理函数

	入口参数：	无

	返回参数：	无
**************************************************************************************/
void GPIOA_IRQHandler(void)
{
    if(CW_GPIOA->ISR_f.PIN10)

    {
        GPIOA_INTFLAG_CLR(bv10);//清除CW_GPIO中断标志
    }

}

/**************************************************************************************
	函数名称：	GPIO_User_Init

	函数功能：	初始化用户定义的GPIO端口

	入口参数：	无

	返回参数：	无
**************************************************************************************/
void GPIO_User_Init()
{
    GPIO_InitTypeDef	GPIO_InitStruct; // GPIO初始化结构体

    __RCC_GPIOA_CLK_ENABLE();// 使能GPIO时钟
    __RCC_GPIOB_CLK_ENABLE();
    __RCC_GPIOC_CLK_ENABLE();

    GPIO_InitStruct.Pins = ESP8266_EN_PIN;			// PB14作为ESP8266的启停开关
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;		//推挽输出
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;		// 输出速度高
    GPIO_Init(ESP8266_EN_PORT, &GPIO_InitStruct);			// 初始化

    GPIO_WritePin(ESP8266_EN_PORT,ESP8266_EN_PIN,GPIO_Pin_RESET); //默认关闭ESP8266

    GPIO_InitStruct.Pins = ESP8266_PIN;				// PB15作为检测ESP8266状态的引脚
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;			//浮空输入
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;		// 输出速度高
    GPIO_Init(ESP8266_PORT, &GPIO_InitStruct);		// 初始化
	
	GPIO_InitStruct.Pins = GPIO_PIN_13;				// PB13作为LDO的启停开关
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;		//推挽输出
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;		// 输出速度高
    GPIO_Init(CW_GPIOB, &GPIO_InitStruct);			// 初始化
	GPIO_WritePin(CW_GPIOB,GPIO_PIN_13,GPIO_Pin_SET);  //默认开启LDO
	
	GPIO_InitStruct.Pins = SCL_PIN|SDA_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	GPIO_Init(CW_GPIOA, &GPIO_InitStruct);
}


/**************************************************************************************
	函数名称：	main

	函数功能：	程序主函数

	入口参数：	无

	返回参数：	无
**************************************************************************************/
int32_t main(void)
{
    board_init();	// 开发板初始化

    //屏幕初始化
    Lcd_Init();  //初始化LCD屏
    Lcd_Clear(BLACK);

    uart1_init(115200);	// 串口1波特率115200
    GPIO_User_Init(); //GPIO初始化
    BTIM1_Init();
    BTIM2_Init();
	GTIM3_PWM_Init();
    Key_Init();
    //上电后设ESP8266为无 Wi-Fi 模式，关闭自动连接WIFI（省电）
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
		
		//操作符在主菜单
		if(in_sub_menu_flag == 0&& in_progress_flag ==0)
		{
			switch(btn)
		{
			//向上
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
			//向下
			case 1:
				Restore_Main_Menu_Button(main_menu_index);
				main_menu_index = (main_menu_index+1)%(MAIN_MENU_INDEX+1);
				sub_menu_index = 0;
				Choose_Main_Function();
				Show_Sub_Menu();
				break;
			//向右
			case 3:
				in_sub_menu_flag = 1;
				Choose_Sub_Function();
				break;
		}
		}
		//操作符在子菜单
		else if(in_sub_menu_flag == 1 && in_progress_flag ==0)
		{
			switch(btn){
			//向上
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
			//向下
			case 1:
				Restore_Sub_Menu_Button(sub_menu_index);
				sub_menu_index = (sub_menu_index+1)%(sub_menu_maxidx[main_menu_index]+1);
				Choose_Sub_Function();
				break;
			//向左
			case 2:
				in_sub_menu_flag = 0;
				Restore_Sub_Menu_Button(sub_menu_index);
				break;
			case 4:
				Run_Function();
				break;
			}
		}
		
		//操作左键返回主菜单
		if(in_progress_flag == 1 && btn == 2)
		{
			Lcd_Clear(BLACK);
			Show_Main_Menu();
			in_progress_flag = 0;
		}
		
		//若ESP8266状态发生变化
		if(ESP8266_Last_Status!=ESP8266_Status && in_progress_flag == 0)
		{
			ESP8266_Last_Status=ESP8266_Status;
			if(ESP8266_Status==1)
				ESP8266_Reset(0);
			else
			{
				FUN[2][0] = Connect_WIFI;
				FUN[3][0] = MQTT_Setup;
				sub_menu_button[2][0]="连接WIFI  ";
				sub_menu_button[3][0]="连接MQTT  ";
				mqtt_status = 0;
				WIFI_Status = 0;
			}
			Show_Main_Menu();
		}
		
		if(bat_cnt>=30)//每30s刷新一次电池电量
		{
			Show_Status_Bar();
			bat_cnt=0;
		}
    }
}
