#include "HX711.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
void HX711_Init(void) //初始化HX711
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //使能PB口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;  //SCK=PB.12
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);   //初始化GPIOB.12

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;  //DOUT
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //
	GPIO_Init(GPIOB, &GPIO_InitStructure);  //初始化GPIOB.13
}


unsigned long Read_HX711(void)  //读HX711输出数据
{
	unsigned long Count;
	unsigned char i;
	unsigned int a=0;
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);    //SCK=0 // 使能 AD （ PD_SCK  置低）
	Count=0;
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)) //AD 转换未结束则等待，否则开始读取
	{
		a++;
		if(a>180000)
		{
			printf("压力传感器已损坏。。。");
			return 0;
		}
	}
	//printf("A:%d\n",a);	
	delay_us(1);
	for (i=0;i<24;i++)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_12); //PD_SCK  置高（发送脉冲）
		Count=Count<<1; // 下降沿来时变量 Count 左移一位，右侧补零
		delay_us(1);
		GPIO_ResetBits(GPIOB,GPIO_Pin_12); //PD_SCK  置低
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)) 
		Count++;	
		delay_us(1);
	}
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
	Count=Count^0x800000;// 第 25 个脉冲下降沿来时，转换数据
	delay_us(1);
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	delay_us(1);
	return Count;

} 


