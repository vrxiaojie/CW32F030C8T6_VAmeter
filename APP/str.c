/*
 * 哔哩哔哩账号		: 	VR小杰      	https://space.bilibili.com/11526854
 * 嘉立创开源广场	: 	vrxiaojie   https://oshwhub.com/vrxiaojie/
 *
 *   Date           Author
 * 2024-08-10     VRxiaojie
 */
#include "str.h"

static char table[]={'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

/**************************************************************************************
	函数名称：	num2char

	函数功能：	浮点数转字符

	入口参数：	str——传入的数组
				number——数字
				g——小数点前位数
				l——小数点后位数

	返回参数：	无
**************************************************************************************/
void num2char(char *str, double number, uint8_t g,  uint8_t l)
{
	 uint8_t i;
	int temp = number/1;
	double t2 = 0.0;
	for (i = 1; i<=g; i++)
	{
		if (temp==0)
			str[g-i] = table[0];
		else
			str[g-i] = table[temp%10];
		temp = temp/10;
	}
	*(str+g) = '.';
	temp = 0;
	t2 = number;
	for(i=1; i<=l; i++)
	{
		temp = t2*10;
		str[g+i] = table[temp%10];
		t2 = t2*10;
	}
	//*(str+g+l+1) = '\0';
}
