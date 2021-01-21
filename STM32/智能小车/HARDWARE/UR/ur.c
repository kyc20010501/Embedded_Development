/*
#include "ur.h"
#include "sys.h"

void UR_Init(u16 arr,u16 psc) {
	
	RCC->ABP1ENR |= 1<<0;
	
	RCC->ABP2ENR |= 1<<2;
	RCC->ABP2ENR |= 1<<3;
	RCC->ABP2ENR |= 1<<4;
	
	GPIOA->CRL &= 0XFFFF0000;
	GPIOA->CRL |= 0X00008888;
	
	GPIOA->ODR |= 0<<0;
	GPIOA->ODR |= 0<<1;
	GPIOA->ODR |= 0<<2;
	GPIOA->ODR |= 0<<3;
	
	GPIOA->CRL &= 0X000FFFFF;
	GPIOA->CRL |= 0X33300000;
	
	GPIOA->ODR |= 1<<5;
	GPIOA->ODR |= 1<<6;
	GPIOA->ODR |= 1<<7;
	
	TIM2->ARR = arr;
	TIM2->PSC = psc;
	
	TIM2->CCMR1 |= 1<<0;
	TIM2->CCMR1 |= 1<<4;
	TIM2->CCMR1 |= 0<<2;
	
	TIM2->CCER |= 0<<1;
	TIM2->CCER |= 1<<0;
	
	TIM2->CCMR1 |= 1<<8;
	TIM2->CCMR1 |= 1<<12;
	TIM2->CCMR1 |= 0<<10;
	
	TIM2->CCER |= 0<<5;
	TIM2->CCER |= 1<<4;
	
	TIM2->CCMR2 |= 1<<0;
	TIM2->CCMR2 |= 1<<4;
	TIM2->CCMR2 |= 0<<2;
	
	TIM2->CCER |= 0<<9;
	TIM2->CCER |= 1<<8;
	
	TIM2->CCMR2 |= 1<<8;
	TIM2->CCMR2 |= 1<<12;
	TIM2->CCMR2 |= 0<<10;
	
	TIM2->CCER |= 0<<13;
	TIM2->CCER |= 1<<12;
	
	TIM2->DIER |= 1<<1;
	TIM2->DIER |= 1<<2;
	TIM2->DIER |= 1<<3;
	TIM2->DIER |= 1<<4;
	TIM2->DIER |= 1<<0;
	TIM2->CR1 |= 0X01;
	
	MY_NVIC_Init(2,0,TIM2_IRQn,2); 
}
*/
