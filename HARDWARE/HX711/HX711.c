#include "HX711.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
void HX711_Init(void) //��ʼ��HX711
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //ʹ��PB��ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;  //SCK=PB.12
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);   //��ʼ��GPIOB.12

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;  //DOUT
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //
	GPIO_Init(GPIOB, &GPIO_InitStructure);  //��ʼ��GPIOB.13
}


unsigned long Read_HX711(void)  //��HX711�������
{
	unsigned long Count;
	unsigned char i;
	unsigned int a=0;
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);    //SCK=0 // ʹ�� AD �� PD_SCK  �õͣ�
	Count=0;
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)) //AD ת��δ������ȴ�������ʼ��ȡ
	{
		a++;
		if(a>180000)
		{
			printf("ѹ�����������𻵡�����");
			return 0;
		}
	}
	//printf("A:%d\n",a);	
	delay_us(1);
	for (i=0;i<24;i++)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_12); //PD_SCK  �øߣ��������壩
		Count=Count<<1; // �½�����ʱ���� Count ����һλ���Ҳಹ��
		delay_us(1);
		GPIO_ResetBits(GPIOB,GPIO_Pin_12); //PD_SCK  �õ�
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)) 
		Count++;	
		delay_us(1);
	}
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
	Count=Count^0x800000;// �� 25 �������½�����ʱ��ת������
	delay_us(1);
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	delay_us(1);
	return Count;

} 


