#ifndef __USART_H
#define __USART_H
#include "sys.h"
#include "stdio.h"	 

#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
#define USART2_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART2_RX 			1		//使能（1）/禁止（0）串口1接收	 
#define UART4_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_UART4_RX 			1		//使能（1）/禁止（0）串口1接收

#define UART2_SendLR() UART2_SendString("\r\n")	  	
//#define UART1_SendLR() UART1_SendString("\r\n")

extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记
extern u8  USART2_RX_BUF[USART2_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART2_RX_STA;         		//接收状态标记		
extern u8  UART4_RX_BUF[UART4_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 UART4_RX_STA;         		//接收状态标记

void uart_init(u32 pclk2,u32 bound);
void uart2_init( u32 bound );
void uart4_init( u32 bound );

//void UART1_SendString(char* s);
void UART2_SendString(char* s);
//void UART1_SendLR(void);
//void UART2_SendLR(void);
#endif	   
















