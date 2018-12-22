#ifndef __DELAY_H
#define __DELAY_H

#include "MyType.h"

void delay_ms(u16 nms);
void delay_us(u32 nus);
void Delay(u32 count);
void Delay_Init(u8 SYSCLK);

extern u16 fac_ms;//全局变量
extern u8 fac_us;//全局变量


#endif

