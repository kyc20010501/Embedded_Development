#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "wheel.h"
#include "inred.h"
#include "key.h"
#include "led.h"
#include "hc6.h"
#include "bluetooth.h"

//∫ÏÕ‚—≠º£
void GO1(void) {
	while(1) {
	
	if(PAin(7)==0 && PAin(11)==0)	{
		GO();
		printf("Go \r\n");
	}
	
	else if(PAin(7)==1 && PAin(11)==0) {
		turnLEFT();
		printf("R \r\n");
		delay_ms(50);
	}
				
	else if(PAin(7)==0 && PAin(11)==1) {
		turnRIGHT();
		printf("L   \r\n");
		delay_ms(50);
	}
	
	else
		GO();
}
	LED0 = 1;
LED1 = 1;
}

//
void GO2(void) {
	while(1)
	Ultrasound();
}
//¿∂—¿“£øÿ
void GO3(void) {
	printf("blue");
}

int main(void) {
	int key;
	JTAG_Set(10);
	
	Stm32_Clock_Init(9);
	uart_init(72,9600);
	uart3_init(36,9600);
	delay_init(72);
	Wheel_Init(7199,0);
	Inred_Init();
	LED_Init();
	KEY_Init();
	HCSR04_Init(0XFFFF,72-1);
	
	left_PWM = 5000;
	right_PWM = 3400;
	
	while(1)
	{
		key=KEY_Scan(0);
	switch(key) {
			case	KEY0_PRES:
				 GO1();
				break;
			case KEY1_PRES:
				GO2();
				break;
			case WKUP_PRES:				
				GO3();
				break;
			default:
				delay_ms(10);	
		}
	}
}

