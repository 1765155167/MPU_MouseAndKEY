#include "usart.h"	 
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "led.h"
#include "key.h"  
#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h" 
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "math.h"
#include "gsm.h"
 
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定向fputc函数
//printf的输出，指向fputc，由fputc输出到串口
//这里使用串口1(USART1)输出printf信息（UART4）
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//等待上一次串口数据发送完成  
	USART1->DR = (u8) ch;      	//写DR,串口1将发送数据
	return ch;
}
#endif 
//end
//////////////////////////////////////////////////////////////////

#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	  
  
void USART1_IRQHandler(void)
{
	u8 res;	

	if(USART1->SR&(1<<5))	//接收到数据
	{	 
		res=USART1->DR; 
		//printf("%c",res);
		if((USART_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART_RX_STA&0x4000)//接收到了0x0d
			{
				if(res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 
			}else //还没收到0X0D
			{	
				if(res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=res;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}  		 									     
	}

} 
#endif		
#if EN_USART2_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART2_RX_BUF[USART2_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART2_RX_STA=0;       //接收状态标记	  
#endif	


//初始化IO 串口1
//pclk2:PCLK2时钟频率(Mhz)
//bound:波特率 
void uart_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   //使能PORTA口时钟  
	RCC->APB2ENR|=1<<14;  //使能串口时钟 
	GPIOA->CRH&=0XFFFFF00F;//IO状态设置
	GPIOA->CRH|=0X000008B0;//IO状态设置 
	RCC->APB2RSTR|=1<<14;   //复位串口1
	RCC->APB2RSTR&=~(1<<14);//停止复位	   	   
	//波特率设置
 	USART1->BRR=mantissa; // 波特率设置	 
	USART1->CR1|=0X200C;  //1位停止,无校验位.
#if EN_USART1_RX		  //如果使能了接收
	//使能接收中断 
	USART1->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
	MY_NVIC_Init(3,3,USART1_IRQn,2);//组2，最低优先级 
#endif
}
/*******************************************************************************
* 函数名  : USART2_Init_Config
* 描述    : USART1初始化配置
* 输入    : bound：波特率(常用：2400、4800、9600、19200、38400、115200等)
* 输出    : 无
* 返回    : 无 
* 说明    : 无
*******************************************************************************/
void uart2_init(u32 bound)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	/*使能USART2外设时钟*/  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	/*复位串口2*/
 	USART_DeInit(USART2);  
	
	/*USART2_GPIO初始化设置*/ 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;			//USART2_TXD(PA.2)     
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//复用推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//设置引脚输出最大速率为50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);				//调用库函数中的GPIO初始化函数，初始化USART1_TXD(PA.9)  
   
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				//USART2_RXD(PA.3)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);					//调用库函数中的GPIO初始化函数，初始化USART1_RXD(PA.10)


   /*USART2 初始化设置*/
	USART_InitStructure.USART_BaudRate = bound;										//设置波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//1个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;								//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//工作模式设置为收发模式
  USART_Init(USART2, &USART_InitStructure);										//初始化串口2

   /*Usart1 NVIC配置*/
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;	//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//从优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);							//根据指定的参数初始化VIC寄存器 
	  
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);			//使能串口2接收中断

  USART_Cmd(USART2, ENABLE);                    			//使能串口 
	USART_ClearFlag(USART2, USART_FLAG_TC);					//清除发送完成标志
}
/*UART4 TX:C10  UART4 RX:C11*/
void uart4_init( u32 bound )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;        

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE );
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE );

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //UART4 TX:C10；
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出；
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure); //端口；
			
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //UART4 RX:C11；
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入；
	GPIO_Init(GPIOC, &GPIO_InitStructure); //端口；

	USART_InitStructure.USART_BaudRate = bound; //波特率；
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //数据位8位；
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //停止位1位；
	USART_InitStructure.USART_Parity = USART_Parity_No ; //无校验位；
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	//无硬件流控；
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	//收发模式；
	USART_Init(UART4, &USART_InitStructure);//配置串口参数；

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置中断组，4位抢占优先级，4位响应优先级；

	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn; //中断号；
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级；
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //响应优先级；
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
	USART_Cmd(UART4, ENABLE); //使能串口；
}

/*******************************************************************************
* 函数名  : UART2_SendString
* 描述    : USART2发送字符串
* 输入    : *s字符串指针
* 输出    : 无
* 返回    : 无 
* 说明    : 无
*******************************************************************************/
void UART2_SendString(char* s)
{
	while(*s)//检测字符串结束符
	{
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET); 
		USART_SendData(USART2 ,*s++);//发送当前字符
	}
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET);
}

void USART4_Send_Byte(u8 Data) //发送一个字节；
{
USART_SendData(USART3,Data);
while( USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET );
}

void USART4_Send_String(u8 *Data) //发送字符串；
{
while(*Data)
USART4_Send_Byte(*Data++);
}

