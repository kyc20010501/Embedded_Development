#include "bluetooth.h"	 
#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "wheel.h"
#include "stdio.h"
 
 
u8 BTFlag;
#if EN_USART3_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART3_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART3_RX_STA=0;       //接收状态标记	  
  
void USART3_IRQHandler(void)
{
	u8 res;	
	//delay_ms(50);
	if(USART3->SR&(1<<5))	//接收到数据
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
 
 
//初始化IO 串口3
//pclk2:PCLK2时钟频率(Mhz)
//bound:波特率 
void uart3_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<3;   //使能PORTB口时钟  
	RCC->APB1ENR|=1<<18;  //使能串口3时钟 
	GPIOB->CRH&=0XFFFF00FF;//IO状态设置
	GPIOB->CRH|=0X00008B00;//IO状态设置 
		  
	RCC->APB2RSTR|=1<<18;   //复位串口3
	RCC->APB2RSTR&=~(1<<18);//停止复位	 
	
	//波特率设置
 	USART3->BRR=mantissa; // 波特率设置	 
	USART3->CR1|=0X200C;  //1位停止,无校验位.
//#if EN_USART1_RX		  //如果使能了接收
	//使能接收中断
	//USART3->CR1|=1<<8;    //PE中断使能
	USART3->CR1|=1<<5;    //接收缓冲区非空中断使能	
	USART3_RX_STA=0;	
	MY_NVIC_Init(3,3,USART3_IRQn,2);//组2，最低优先级 
//#endif


}



