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
 
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef�� d in stdio.h. */ 
FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc����
//printf�������ָ��fputc����fputc���������
//����ʹ�ô���1(USART1)���printf��Ϣ��UART4��
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//�ȴ���һ�δ������ݷ������  
	USART1->DR = (u8) ch;      	//дDR,����1����������
	return ch;
}
#endif 
//end
//////////////////////////////////////////////////////////////////

#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	  
  
void USART1_IRQHandler(void)
{
	u8 res;	

	if(USART1->SR&(1<<5))	//���յ�����
	{	 
		res=USART1->DR; 
		//printf("%c",res);
		if((USART_RX_STA&0x8000)==0)//����δ���
		{
			if(USART_RX_STA&0x4000)//���յ���0x0d
			{
				if(res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else USART_RX_STA|=0x8000;	//��������� 
			}else //��û�յ�0X0D
			{	
				if(res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=res;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}  		 									     
	}

} 
#endif		
#if EN_USART2_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART2_RX_BUF[USART2_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART2_RX_STA=0;       //����״̬���	  
#endif	


//��ʼ��IO ����1
//pclk2:PCLK2ʱ��Ƶ��(Mhz)
//bound:������ 
void uart_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   //ʹ��PORTA��ʱ��  
	RCC->APB2ENR|=1<<14;  //ʹ�ܴ���ʱ�� 
	GPIOA->CRH&=0XFFFFF00F;//IO״̬����
	GPIOA->CRH|=0X000008B0;//IO״̬���� 
	RCC->APB2RSTR|=1<<14;   //��λ����1
	RCC->APB2RSTR&=~(1<<14);//ֹͣ��λ	   	   
	//����������
 	USART1->BRR=mantissa; // ����������	 
	USART1->CR1|=0X200C;  //1λֹͣ,��У��λ.
#if EN_USART1_RX		  //���ʹ���˽���
	//ʹ�ܽ����ж� 
	USART1->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(3,3,USART1_IRQn,2);//��2��������ȼ� 
#endif
}
/*******************************************************************************
* ������  : USART2_Init_Config
* ����    : USART1��ʼ������
* ����    : bound��������(���ã�2400��4800��9600��19200��38400��115200��)
* ���    : ��
* ����    : �� 
* ˵��    : ��
*******************************************************************************/
void uart2_init(u32 bound)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	/*ʹ��USART2����ʱ��*/  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	/*��λ����2*/
 	USART_DeInit(USART2);  
	
	/*USART2_GPIO��ʼ������*/ 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;			//USART2_TXD(PA.2)     
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//�����������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//������������������Ϊ50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);				//���ÿ⺯���е�GPIO��ʼ����������ʼ��USART1_TXD(PA.9)  
   
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				//USART2_RXD(PA.3)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);					//���ÿ⺯���е�GPIO��ʼ����������ʼ��USART1_RXD(PA.10)


   /*USART2 ��ʼ������*/
	USART_InitStructure.USART_BaudRate = bound;										//���ò�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;								//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//����ģʽ����Ϊ�շ�ģʽ
  USART_Init(USART2, &USART_InitStructure);										//��ʼ������2

   /*Usart1 NVIC����*/
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;	//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);							//����ָ���Ĳ�����ʼ��VIC�Ĵ��� 
	  
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);			//ʹ�ܴ���2�����ж�

  USART_Cmd(USART2, ENABLE);                    			//ʹ�ܴ��� 
	USART_ClearFlag(USART2, USART_FLAG_TC);					//���������ɱ�־
}
/*UART4 TX:C10  UART4 RX:C11*/
void uart4_init( u32 bound )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;        

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE );
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE );

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //UART4 TX:C10��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�������������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure); //�˿ڣ�
			
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //UART4 RX:C11��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //�������룻
	GPIO_Init(GPIOC, &GPIO_InitStructure); //�˿ڣ�

	USART_InitStructure.USART_BaudRate = bound; //�����ʣ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //����λ8λ��
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //ֹͣλ1λ��
	USART_InitStructure.USART_Parity = USART_Parity_No ; //��У��λ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	//��Ӳ�����أ�
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	//�շ�ģʽ��
	USART_Init(UART4, &USART_InitStructure);//���ô��ڲ�����

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //�����ж��飬4λ��ռ���ȼ���4λ��Ӧ���ȼ���

	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn; //�жϺţ�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ���
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //��Ӧ���ȼ���
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
	USART_Cmd(UART4, ENABLE); //ʹ�ܴ��ڣ�
}

/*******************************************************************************
* ������  : UART2_SendString
* ����    : USART2�����ַ���
* ����    : *s�ַ���ָ��
* ���    : ��
* ����    : �� 
* ˵��    : ��
*******************************************************************************/
void UART2_SendString(char* s)
{
	while(*s)//����ַ���������
	{
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET); 
		USART_SendData(USART2 ,*s++);//���͵�ǰ�ַ�
	}
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET);
}

void USART4_Send_Byte(u8 Data) //����һ���ֽڣ�
{
USART_SendData(USART3,Data);
while( USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET );
}

void USART4_Send_String(u8 *Data) //�����ַ�����
{
while(*Data)
USART4_Send_Byte(*Data++);
}

