/*
��귢�͸�PC������ÿ��4���ֽ� 
BYTE1 BYTE2 BYTE3 BYTE4 
����ֱ��ǣ� Mouse_Buffer[4]
BYTE1 -- 
       |--bit7:   1   ��ʾ   Y   ����ı仯��������256   ~   255�ķ�Χ,0��ʾû�����  
       |--bit6:   1   ��ʾ   X   ����ı仯��������256   ~   255�ķ�Χ��0��ʾû�����  
       |--bit5:   Y   ����仯�ķ���λ��1��ʾ����������������ƶ�  
       |--bit4:   X   ����仯�ķ���λ��1��ʾ����������������ƶ�  
       |--bit3:     ��Ϊ1  
       |--bit2:     1��ʾ�м�����  
       |--bit1:     1��ʾ�Ҽ�����  
       |--bit0:     1��ʾ�������  
BYTE2 -- X����仯������byte��bit4���9λ������,������ʾ�����ƣ����������ơ��ò����ʾ�仯��  
BYTE3 -- Y����仯������byte��bit5���9λ��������������ʾ�����ƣ����������ơ��ò����ʾ�仯�� 
BYTE4 -- ���ֱ仯�� 
����BYTE2 BYTE3�������ԣ�BYTE1��4-7ȫΪ0��ʱ��BYTE2 BYTE3��������ʾ����ƶ����� 

���̷��͸�PC������ÿ��8���ֽ� 
BYTE1 BYTE2 BYTE3 BYTE4 BYTE5 BYTE6 BYTE7 BYTE8 
����ֱ��ǣ� 
BYTE1 -- 
       |--bit0:   Left Control�Ƿ��£�����Ϊ1  
       |--bit1:   Left Shift  �Ƿ��£�����Ϊ1  
       |--bit2:   Left Alt    �Ƿ��£�����Ϊ1  
       |--bit3:   Left GUI    �Ƿ��£�����Ϊ1  
       |--bit4:   Right Control�Ƿ��£�����Ϊ1   
       |--bit5:   Right Shift �Ƿ��£�����Ϊ1  
       |--bit6:   Right Alt   �Ƿ��£�����Ϊ1  
       |--bit7:   Right GUI   �Ƿ��£�����Ϊ1  
BYTE2 -- �ݲ�������еĵط�˵�Ǳ���λ 
BYTE3--BYTE8 -- ������Ϊ��ͨ���� 
���̾������ԡ� 
���磺���̷���һ֡����   01 00 00 00 0x06 00 00 00 
��ʾͬʱ������ Left Control + ��c�� */
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

u8 KEY()//��ȡѹ����������ֵ
{	 
	float d=0;
	static u8 i1=0,d1;//�������ɿ���־
	d=Read_HX711()/980.0-8571.75;
	//printf("%.2f\n",d);
	if(i1==0) d1=d,i1=2;
	if(d-d1>8)
	{
		return KEY0_PRES;
	}
 	return 0;// �ް�������
}

int main(void)
{ 
		u8 key,a=0,i=0,c;
		u8 x0=0x00;		 	//X����仯������byte��bit4���9λ������,������ʾ�����ƣ����������ơ��ò����ʾ�仯��  						
		u8 y0=0x00;			//Y����仯������byte��bit5���9λ��������������ʾ�����ƣ����������ơ��ò����ʾ�仯�� 
		u8 keysta=0x00;		//BYTE1
		float pitch,roll,yaw; 		//ŷ����
		Stm32_Clock_Init(9);		//ϵͳʱ������
		delay_init(72);	//��ʱ������ʼ��	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�жϷ�������	 
		uart_init(72,115200);//���ڳ�ʼ��Ϊ115200
		delay_ms(300);
		uart2_init(115200);
		delay_ms(300);
  	uart4_init(9600);//���ڳ�ʼ��Ϊ9600
		led_init();		//��ʼ����LED���ӵ�Ӳ���ӿ�
		KEY_Init();		//������ʼ��		 	  						    	   
		delay_ms(1800);
	  printf("USB�ȶϿ�\n");
		USB_Port_Set(0); 	//USB�ȶϿ�
		delay_ms(300);
		USB_Port_Set(1);	//USB�ٴ�����
	  LED1=1;
		delay_ms(3000);
		printf("USB�ٴ�����\n");
	  //USB����
		USB_Interrupts_Config();    
		Set_USBClock();   
		USB_Init();	  
		delay_ms(200);
		printf("��ʼ��MPU6050\n");
		MPU_Init();					//��ʼ��MPU6050
		while(mpu_dmp_init())
		{
			delay_ms(200);
			delay_ms(200);
			i++;
			if(i>10){ printf("MUPerror\n");break;}
		}
		if(i<=5)printf("MPU��ʼ���ɹ�\n");
		delay_ms(300);
		printf("GSMģ��ע��\n");
		if(Wait_CREG())    //��ѯ�ȴ�ģ��ע��ɹ�
		{
			 printf("GSMģ��ע��ɹ�\n");
			 gsm=1;
			 Set_Pdu_Mode();//���ö���ΪPDUģʽ
			 printf("����ΪPDUģʽ�ɹ�\n");
		}else printf("GSMģ��ע��ʧ��\n"); 
		LED1=0;
		LED0=1;
		if(gsm)Send_Pdu_Sms(),printf("����һ������Ϣ�ɹ�");//����һ������Ϣ
		while(1)
		{	 
			c++;
			mpu_dmp_get_data(&pitch,&roll,&yaw);
		  printf("��ʵֵ��%.1f %.1f %.1f\n",pitch,roll,yaw);
			if(yaw>=45&&a==0) a=1,LED1=0;
			if(a==1&&yaw<5)  a=2,LED1=1;//����//����	
			
			if(a==2&&yaw>45)  a=3,LED1=0;
			if(a==3&&yaw<5)  a=0,LED1=0;//����//����				
			
			if(c>100)key=KEY(),c=0;
			if(a!=2&&key==KEY0_PRES)keysta=keysta|0X01;		 										//����������    
			if(a!=2&&yaw<-25)keysta=keysta|0X02;	            								//��������Ҽ�
			if(a!=2&&pitch>MY)  y0=(int)(pitch-MY)/3;								//����,
			if(a!=2&&pitch<-MY) y0=(int)(pitch+MY)/3;								//����,
			if(a!=2&&roll>MX)   x0=(int)(roll-MX)/3;								//����,
			if(a!=2&&roll<-MX)  x0=(int)(roll+MX)/3;								//����,
			if(a!=2&&key==KEY1_PRES&&gsm)Send_Pdu_Sms(),printf("����һ������Ϣ�ɹ�");			//����һ������Ϣ
			Joystick_Send_mouse(keysta,x0,y0,0); 							//�������ݵ����� 
			keysta=0;x0=0;y0=0;key=0;
		}	   			
}


#if EN_UART4_RX   //���ʹ���˽���
u8 UART4_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u16 UART4_RX_STA=0;       //����״̬���	  
u8 buf[8]={00,00,00,00,00,00,00,00},z=0;
u8 t;
void UART4_IRQHandler(void)
{
	if(UART4->SR&(1<<5))	//���յ�����
	{	 
		UART4_RX_BUF[0]=UART4->DR; 		
		UART4_RX_STA|=0x8000;
		//����
			if(UART4_RX_STA&0x8000)
			{	 
				printf("%c",UART4_RX_BUF[0]);
				UART4_RX_STA=0; 
				switch(UART4_RX_BUF[0])
				{
					case 0xe1:if(z==1){																									//֣���ṤҵѧԺ  0x1d, 0x1d, 0x14, 0x0a, 0x1c, 0x1b, 0x1c,
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
					case 0xe2:if(z==1){																									//Ϊ֮���� ��Ϊ����
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
					case 0xe3:if(gsm)Send_Pdu_Sms();printf("����һ������Ϣ�ɹ�");break;		//����һ������Ϣ
					case 0x02:z=1;LED0=0;printf("�򿪼��̳ɹ�");break;											//�򿪼���	
					case 0x03:z=0;LED0=1;printf("�رռ��̳ɹ�");break;											//�رռ���	
					default:if(z==1){																											//��ͨ����	
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

