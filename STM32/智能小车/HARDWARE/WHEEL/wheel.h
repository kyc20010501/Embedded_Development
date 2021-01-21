#ifndef __WHEEL_H
#define __WHEEL_H

#include "sys.h"

//TB6612����
#define LEFT PAout(2)
#define LIN1 PAout(1)            
#define LIN2 PAout(4)
 
#define RIGHT PAout(3)
#define RIN1 PAout(5)
#define RIN2 PAout(6)

//#define STBY PBout(0)

#define left_PWM  TIM2->CCR3//ռ�ձ�
#define right_PWM  TIM2->CCR4//ռ�ձ� 

void Wheel_Init(u16 arr,u16 psc);

void GO(void);//ִ��ֱ�в���
void STOP(void);
void BACK(void);//ִ�к��˲���
void turnLEFT(void);//ִ����ת����
void turnRIGHT(void);//ִ����ת����

#endif
