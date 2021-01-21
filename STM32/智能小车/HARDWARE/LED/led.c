#include "led.h"
#include "stm32f10x.h"

void LED_Init(void) {
	
	RCC->APB2ENR|=1<<2;
	RCC->APB2ENR|=1<<5;
	
	GPIOA->CRH&=0xFFFFFFF0;
	GPIOA->CRH|=0x00000003;
	GPIOA->ODR|=1<<8;
	
	GPIOD->CRL&=0xFFFFF0FF;
	GPIOD->CRL|=0x00000300;
	GPIOD->ODR|=1<<2;
}
