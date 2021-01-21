#include "wheel.h"
#include "delay.h"

void Wheel_Init(u16 arr,u16 psc) {
	
	RCC->APB1ENR |=1<<0;//TIM2时钟使能
	
	RCC->APB2ENR |=1<<2;
	
	GPIOA->CRL &=0XFF000000;//PA3,4复用输出
	GPIOA->CRL |=0X0333BB30;//PA1,2,5,6推挽输出

	TIM2->ARR=arr;//自动重装载值
	TIM2->PSC=psc;//预分频系数
	
	TIM2->CCMR2|=7<<4; //CH3 PWM2 模式//比较捕获模式配置寄存器
	TIM2->CCER|=1<<8;   	//输出使能	//捕获/比较使能寄存器
 	
	TIM2->CCMR2|=7<<12; //CH4 PWM2 模式//比较捕获模式配置寄存器
	TIM2->CCER|=1<<12;   	//输出使能	//捕获/比较使能寄存器
 	
	
	TIM2->CR1 |=0X0001;//使能定时器2
}

void GO(void) {
	LIN1=0;LIN2=1;
	RIN1=0;RIN2=1;
}

void STOP(void) {
	LIN1=0;LIN2=0;
	RIN1=0;RIN2=0;
}

void BACK(void) {
	LIN1=1;LIN2=0;
	RIN1=1;RIN2=0;
}

void turnLEFT(void) {
	//TURN LEFT/L0R1
	LIN1=0;LIN2=1;
	RIN1=1;RIN2=0;
}

void turnRIGHT(void) {
	//TURN RIGHT/L1R0
	LIN1=1;LIN2=0;
	RIN1=0;RIN2=1;
}
