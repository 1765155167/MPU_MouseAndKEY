#ifndef _KEY_H
#define _KEY_H
#include "sys.h"

#define KEY0 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)
#define KEY1 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)
#define WK_UP GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)


#define KEY0_PRES 	1	//KEY0按下
#define KEY1_PRES	  2	//KEY1按下
#define WKUP_PRES   3	//KEY_UP按下(即WK_UP/KEY_UP)

void KEY_Init(void); //IO初始化
u8 KEY_Scan(u8 mode);
#endif

