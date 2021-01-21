#ifndef __HC6_H
#define __HC6_H
#include "sys.h"

void HCSR04_Init(u16 arr,u16 psc);
u32 DIS_Init(u8 *STA,u16 VAL);
void Ultrasound(void);

#endif
