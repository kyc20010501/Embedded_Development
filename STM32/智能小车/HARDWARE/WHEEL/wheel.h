#ifndef __WHEEL_H
#define __WHEEL_H

#include "sys.h"

//TB6612连接
#define LEFT PAout(2)
#define LIN1 PAout(1)            
#define LIN2 PAout(4)
 
#define RIGHT PAout(3)
#define RIN1 PAout(5)
#define RIN2 PAout(6)

//#define STBY PBout(0)

#define left_PWM  TIM2->CCR3//占空比
#define right_PWM  TIM2->CCR4//占空比 

void Wheel_Init(u16 arr,u16 psc);

void GO(void);//执行直行操作
void STOP(void);
void BACK(void);//执行后退操作
void turnLEFT(void);//执行左转操作
void turnRIGHT(void);//执行右转操作

#endif
