#include "inred.h"

void Inred_Init(void) {
	RCC->APB2ENR |= 1<<2;
	GPIOA->CRL &= 0X0FFFFFFF;
	GPIOA->CRL |= 0X80000000;
	
	GPIOA->CRH &= 0XFFFF0FFF;
	GPIOA->CRH |= 0X00008000;
}




