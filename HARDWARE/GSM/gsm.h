#ifndef _GSM_H
#define _GSM_H
#include "string.h"
#include "stm32f10x.h"


/*************	���غ�������	**************/
void CLR_Buf2(void);
u8 Find(char *a);
void Second_AT_Command(char *b,char *a,u8 wait_time);
int Wait_CREG(void);
void Set_Pdu_Mode(void);//���ö���ģʽPDU
void Send_Pdu_Sms(void);//����һ�����Ķ���
/*************  �ⲿ�����ͱ�������*****************/


#endif


