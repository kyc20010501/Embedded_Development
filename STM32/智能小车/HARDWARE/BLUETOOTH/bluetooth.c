#include "bluetooth.h"	 
#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "wheel.h"
#include "stdio.h"
 
 
u8 BTFlag;
#if EN_USART3_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART3_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART3_RX_STA=0;       //����״̬���	  
  
void USART3_IRQHandler(void)
{
	u8 res;	
	//delay_ms(50);
	if(USART3->SR&(1<<5))	//���յ�����
	{	
		//delay_ms(10);		
		res=USART3->DR;
		printf("\r\n%d",res);		
		if(res == 1)
		{	
			GO();
			printf("\r\nGO");
		}
		else if(res == 2)	
		{
			BACK();
			printf("\r\nBack");
		}
		else if(res== 3)
		{
			turnLEFT();
			printf("\r\nLeft");			
		}
		else if(res==4)
		{
			turnRIGHT();
			printf("\r\nRight");
		}
		else if(res==5)
		{
			STOP();
			printf("\r\nStop");
		}
	}
} 
#endif		
 
 
//��ʼ��IO ����3
//pclk2:PCLK2ʱ��Ƶ��(Mhz)
//bound:������ 
void uart3_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<3;   //ʹ��PORTB��ʱ��  
	RCC->APB1ENR|=1<<18;  //ʹ�ܴ���3ʱ�� 
	GPIOB->CRH&=0XFFFF00FF;//IO״̬����
	GPIOB->CRH|=0X00008B00;//IO״̬���� 
		  
	RCC->APB2RSTR|=1<<18;   //��λ����3
	RCC->APB2RSTR&=~(1<<18);//ֹͣ��λ	 
	
	//����������
 	USART3->BRR=mantissa; // ����������	 
	USART3->CR1|=0X200C;  //1λֹͣ,��У��λ.
//#if EN_USART1_RX		  //���ʹ���˽���
	//ʹ�ܽ����ж�
	//USART3->CR1|=1<<8;    //PE�ж�ʹ��
	USART3->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	
	USART3_RX_STA=0;	
	MY_NVIC_Init(3,3,USART3_IRQn,2);//��2��������ȼ� 
//#endif


}



