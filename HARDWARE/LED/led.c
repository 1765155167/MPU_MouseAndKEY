#include "led.h"
#include "stm32f10x.h"

void led_init()
{

	GPIO_InitTypeDef GPIO_Init_led;
	
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB , ENABLE );
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOE , ENABLE );
	

	
	GPIO_Init_led.GPIO_Mode= GPIO_Mode_Out_PP;//推挽输出
	GPIO_Init_led.GPIO_Pin=GPIO_Pin_5;
	GPIO_Init_led.GPIO_Speed=	GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_Init_led);//初始化
	GPIO_SetBits(GPIOB,GPIO_Pin_5);//置高
	
	
	GPIO_Init_led.GPIO_Mode= GPIO_Mode_Out_PP;
	GPIO_Init_led.GPIO_Pin=GPIO_Pin_5;
	GPIO_Init_led.GPIO_Speed=	GPIO_Speed_50MHz;
	GPIO_Init(GPIOE,&GPIO_Init_led);
	GPIO_SetBits(GPIOE,GPIO_Pin_5);
}

