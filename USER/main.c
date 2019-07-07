/*
鼠标发送给PC的数据每次4个字节 
BYTE1 BYTE2 BYTE3 BYTE4 
定义分别是： Mouse_Buffer[4]
BYTE1 -- 
       |--bit7:   1   表示   Y   坐标的变化量超出－256   ~   255的范围,0表示没有溢出  
       |--bit6:   1   表示   X   坐标的变化量超出－256   ~   255的范围，0表示没有溢出  
       |--bit5:   Y   坐标变化的符号位，1表示负数，即鼠标向下移动  
       |--bit4:   X   坐标变化的符号位，1表示负数，即鼠标向左移动  
       |--bit3:     恒为1  
       |--bit2:     1表示中键按下  
       |--bit1:     1表示右键按下  
       |--bit0:     1表示左键按下  
BYTE2 -- X坐标变化量，与byte的bit4组成9位符号数,负数表示向左移，正数表右移。用补码表示变化量  
BYTE3 -- Y坐标变化量，与byte的bit5组成9位符号数，负数表示向下移，正数表上移。用补码表示变化量 
BYTE4 -- 滚轮变化。 
对于BYTE2 BYTE3做个测试，BYTE1的4-7全为0的时候，BYTE2 BYTE3的正负表示鼠标移动方向 

键盘发送给PC的数据每次8个字节 
BYTE1 BYTE2 BYTE3 BYTE4 BYTE5 BYTE6 BYTE7 BYTE8 
定义分别是： 
BYTE1 -- 
       |--bit0:   Left Control是否按下，按下为1  
       |--bit1:   Left Shift  是否按下，按下为1  
       |--bit2:   Left Alt    是否按下，按下为1  
       |--bit3:   Left GUI    是否按下，按下为1  
       |--bit4:   Right Control是否按下，按下为1   
       |--bit5:   Right Shift 是否按下，按下为1  
       |--bit6:   Right Alt   是否按下，按下为1  
       |--bit7:   Right GUI   是否按下，按下为1  
BYTE2 -- 暂不清楚，有的地方说是保留位 
BYTE3--BYTE8 -- 这六个为普通按键 
键盘经过测试。 
例如：键盘发送一帧数据   01 00 00 00 0x06 00 00 00 
表示同时按下了 Left Control + ‘c’ */
#include "stdio.h"
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
#include "HX711.h"

#define MX 8
#define MY 8
u8 gsm =0;

u8 KEY()//读取压力传感器的值
{	 
	float d=0;
	static u8 i1=0,d1;//按键按松开标志
	d=Read_HX711()/980.0-8571.75;
	//printf("%.2f\n",d);
	if(i1==0) d1=d,i1=2;
	if(d-d1>8)
	{
		return KEY0_PRES;
	}
 	return 0;// 无按键按下
}

int main(void)
{ 
		u8 key,a=0,i=0,c;
		u8 x0=0x00;		 	//X坐标变化量，与byte的bit4组成9位符号数,负数表示向左移，正数表右移。用补码表示变化量  						
		u8 y0=0x00;			//Y坐标变化量，与byte的bit5组成9位符号数，负数表示向下移，正数表上移。用补码表示变化量 
		u8 keysta=0x00;		//BYTE1
		float pitch,roll,yaw; 		//欧拉角
		Stm32_Clock_Init(9);		//系统时钟设置
		delay_init(72);	//延时函数初始化	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断分组设置	 
		uart_init(72,115200);//串口初始化为115200
		delay_ms(300);
		uart2_init(115200);
		delay_ms(300);
  	uart4_init(9600);//串口初始化为9600
		led_init();		//初始化与LED连接的硬件接口
		KEY_Init();		//按键初始化		 	  						    	   
		delay_ms(1800);
	  printf("USB先断开\n");
		USB_Port_Set(0); 	//USB先断开
		delay_ms(300);
		USB_Port_Set(1);	//USB再次连接
	  LED1=1;
		delay_ms(3000);
		printf("USB再次连接\n");
	  //USB配置
		USB_Interrupts_Config();    
		Set_USBClock();   
		USB_Init();	  
		delay_ms(200);
		printf("初始化MPU6050\n");
		MPU_Init();					//初始化MPU6050
		while(mpu_dmp_init())
		{
			delay_ms(200);
			delay_ms(200);
			i++;
			if(i>10){ printf("MUPerror\n");break;}
		}
		if(i<=5)printf("MPU初始化成功\n");
		delay_ms(300);
		printf("GSM模块注册\n");
		if(Wait_CREG())    //查询等待模块注册成功
		{
			 printf("GSM模块注册成功\n");
			 gsm=1;
			 Set_Pdu_Mode();//设置短信为PDU模式
			 printf("短信为PDU模式成功\n");
		}else printf("GSM模块注册失败\n"); 
		LED1=0;
		LED0=1;
		if(gsm)Send_Pdu_Sms(),printf("发送一条短消息成功");//发送一条短消息
		while(1)
		{	 
			c++;
			mpu_dmp_get_data(&pitch,&roll,&yaw);
		  printf("真实值：%.1f %.1f %.1f\n",pitch,roll,yaw);
			if(yaw>=45&&a==0) a=1,LED1=0;
			if(a==1&&yaw<5)  a=2,LED1=1;//锁定//灯灭	
			
			if(a==2&&yaw>45)  a=3,LED1=0;
			if(a==3&&yaw<5)  a=0,LED1=0;//解锁//灯亮				
			
			if(c>100)key=KEY(),c=0;
			if(a!=2&&key==KEY0_PRES)keysta=keysta|0X01;		 										//发送鼠标左键    
			if(a!=2&&yaw<-25)keysta=keysta|0X02;	            								//发送鼠标右键
			if(a!=2&&pitch>MY)  y0=(int)(pitch-MY)/3;								//上移,
			if(a!=2&&pitch<-MY) y0=(int)(pitch+MY)/3;								//下移,
			if(a!=2&&roll>MX)   x0=(int)(roll-MX)/3;								//右移,
			if(a!=2&&roll<-MX)  x0=(int)(roll+MX)/3;								//左移,
			if(a!=2&&key==KEY1_PRES&&gsm)Send_Pdu_Sms(),printf("发送一条短消息成功");			//发送一条短消息
			Joystick_Send_mouse(keysta,x0,y0,0); 							//发送数据到电脑 
			keysta=0;x0=0;y0=0;key=0;
		}	   			
}


#if EN_UART4_RX   //如果使能了接收
u8 UART4_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
u16 UART4_RX_STA=0;       //接收状态标记	  
u8 buf[8]={00,00,00,00,00,00,00,00},z=0;
u8 t;
void UART4_IRQHandler(void)
{
	if(UART4->SR&(1<<5))	//接收到数据
	{	 
		UART4_RX_BUF[0]=UART4->DR; 		
		UART4_RX_STA|=0x8000;
		//键盘
			if(UART4_RX_STA&0x8000)
			{	 
				printf("%c",UART4_RX_BUF[0]);
				UART4_RX_STA=0; 
				switch(UART4_RX_BUF[0])
				{
					case 0xe1:if(z==1){																									//郑州轻工业学院  0x1d, 0x1d, 0x14, 0x0a, 0x1c, 0x1b, 0x1c,
						Joystick_Send_key(0, 0, 0, 0x1d, 0, 0, 0, 0);//z
						delay_ms(10);
						Joystick_Send_key(0, 0, 0, 0, 0, 0, 0, 0);//z
						delay_ms(10);
						Joystick_Send_key(0, 0, 0, 0, 0x1d, 0, 0, 0);//z
						delay_ms(10);
						Joystick_Send_key(0, 0, 0, 0x14, 0, 0, 0, 0);//q
						delay_ms(10);
						Joystick_Send_key(0, 0, 0, 0x0a, 0, 0, 0, 0);//g
						delay_ms(10);
						Joystick_Send_key(0, 0, 0, 0x1c, 0, 0, 0, 0);//y
						delay_ms(10);
						Joystick_Send_key(0, 0, 0, 0x1b, 0, 0, 0, 0);//x
						delay_ms(10);
						Joystick_Send_key(0, 0, 0, 0x1c, 0, 0, 0, 0);//y
						delay_ms(10);}break;
					case 0xe2:if(z==1){																									//为之则易 不为则难
						Joystick_Send_key(0, 0, 0, 0x1a, 0, 0, 0, 0);//w
						delay_ms(10);
						Joystick_Send_key(0, 0, 0, 0x1d, 0, 0, 0, 0);//z
						delay_ms(10);
						Joystick_Send_key(0, 0, 0, 0, 0, 0, 0, 0);//z
						delay_ms(10);
						Joystick_Send_key(0, 0, 0, 0, 0x1d, 0, 0, 0);//z
						delay_ms(10);
						Joystick_Send_key(0, 0, 0, 0x1c, 0, 0, 0, 0);//y
						delay_ms(10);
						Joystick_Send_key(0, 0, 0, 0x05, 0, 0, 0, 0);//b
						delay_ms(10);
						Joystick_Send_key(0, 0, 0, 0x1a, 0, 0, 0, 0);//w
						delay_ms(10);
						Joystick_Send_key(0, 0, 0, 0x1d, 0, 0, 0, 0);//z
						delay_ms(10);
						Joystick_Send_key(0, 0, 0, 0x11, 0, 0, 0, 0);//n
						delay_ms(10);}break;
					case 0xe3:if(gsm)Send_Pdu_Sms();printf("发送一条短消息成功");break;		//发送一条短消息
					case 0x02:z=1;LED0=0;printf("打开键盘成功");break;											//打开键盘	
					case 0x03:z=0;LED0=1;printf("关闭键盘成功");break;											//关闭键盘	
					default:if(z==1){																											//普通按键	
							buf[4]=UART4_RX_BUF[0];
							Joystick_Send_key( buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7]);
							for(t=0;t<8;t++) buf[t]=0;
							delay_ms(10);}	break;
				}
			}	
			Joystick_Send_key( buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7]);
	}
}  
#endif	

