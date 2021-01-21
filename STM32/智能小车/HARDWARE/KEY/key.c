#include "key.h"
#include "delay.h"

void KEY_Init(void) {	//初始化按键 a0 a15 c5
	RCC->APB2ENR|=1<<2;
	RCC->APB2ENR|=1<<4;
	
	GPIOA->CRL&=0xFFFFFFF0;
	GPIOA->CRL|=0x00000008;
	
	GPIOA->CRH&=0x0FFFFFFF;
	GPIOA->CRH|=0x80000000;
	GPIOA->ODR|=1<<15;
	
	GPIOC->CRL&=0xFF0FFFFF;
	GPIOC->CRL|=0x00800000;
	GPIOC->ODR|=1<<5;
}

int KEY_Scan(u8 mode) {	//扫描按键

	static u8 key_up=1;//按键按松开标志	  
	
	if(KEY0==0||KEY1==0||WK_UP==1) {
		
		delay_ms(10);//去抖动 
		key_up=0;
		
		if(KEY0==0)
			return KEY0_PRES;
		
		else if(KEY1==0)
			return KEY1_PRES;
		
		else if
			(WK_UP==1)return WKUP_PRES; 
	}
	else if(KEY0==1&&KEY1==1&&WK_UP==0)
		key_up=1; 	     
	
	return 0;// 无按键按下
}
