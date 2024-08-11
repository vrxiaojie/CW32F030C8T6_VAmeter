/**
* @file         str.c
* @brief		�����ַ������Զ��庯��Դ�ļ�
* @author		VRxiaojie
* @date		    2024-08-11
* @version	    v1.0
* @note 	 	��������	:		VRС��		https://space.bilibili.com/11526854 <br>
* 				��������Դ�㳡:	vrxiaojie	https://oshwhub.com/vrxiaojie/	
*/

char buffer[300];

#include "str.h"

static char table[]={'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

/**
* @brief		������ת�ַ�
* @param		str ���������
* @param		number ����
* @param		g С����ǰλ��
* @param		l С�����λ��
* @return		��
*/

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

/**
* @brief		�Զ����ƴ���ַ������� ������ʾ����Ļ��
* @param		s1
* @param		s2
* @return		buffer ƴ�Ӻõ��ַ���
*/
char* strcat_new(char* s1,char* s2)
{
    memset(buffer,0,300);
    strcpy(buffer,s1);
    strcat(buffer,s2);
    return buffer;
}
