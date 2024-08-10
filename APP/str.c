/*
 * ���������˺�		: 	VRС��      	https://space.bilibili.com/11526854
 * ��������Դ�㳡	: 	vrxiaojie   https://oshwhub.com/vrxiaojie/
 *
 *   Date           Author
 * 2024-08-10     VRxiaojie
 */
#include "str.h"

static char table[]={'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

/**************************************************************************************
	�������ƣ�	num2char

	�������ܣ�	������ת�ַ�

	��ڲ�����	str�������������
				number��������
				g����С����ǰλ��
				l����С�����λ��

	���ز�����	��
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
