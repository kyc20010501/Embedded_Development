#include "wheel.h"
#include "delay.h"

void Wheel_Init(u16 arr,u16 psc) {
	
	RCC->APB1ENR |=1<<0;//TIM2ʱ��ʹ��
	
	RCC->APB2ENR |=1<<2;
	
	GPIOA->CRL &=0XFF000000;//PA3,4�������
	GPIOA->CRL |=0X0333BB30;//PA1,2,5,6�������

	TIM2->ARR=arr;//�Զ���װ��ֵ
	TIM2->PSC=psc;//Ԥ��Ƶϵ��
	
	TIM2->CCMR2|=7<<4; //CH3 PWM2 ģʽ//�Ƚϲ���ģʽ���üĴ���
	TIM2->CCER|=1<<8;   	//���ʹ��	//����/�Ƚ�ʹ�ܼĴ���
 	
	TIM2->CCMR2|=7<<12; //CH4 PWM2 ģʽ//�Ƚϲ���ģʽ���üĴ���
	TIM2->CCER|=1<<12;   	//���ʹ��	//����/�Ƚ�ʹ�ܼĴ���
 	
	
	TIM2->CR1 |=0X0001;//ʹ�ܶ�ʱ��2
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
