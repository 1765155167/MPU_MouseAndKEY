#ifndef _GSM_H
#define _GSM_H
#include "string.h"
#include "stm32f10x.h"


/*************	本地函数声明	**************/
void CLR_Buf2(void);
u8 Find(char *a);
void Second_AT_Command(char *b,char *a,u8 wait_time);
int Wait_CREG(void);
void Set_Pdu_Mode(void);//设置短信模式PDU
void Send_Pdu_Sms(void);//发送一条中文短信
/*************  外部函数和变量声明*****************/


#endif


