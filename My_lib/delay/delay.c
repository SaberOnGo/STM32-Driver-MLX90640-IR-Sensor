/****************************************
JinMing electronics
https://www.ebay.com/itm/223274591029
****************************************/

#include "delay.h"
#include "stm32f4xx_hal.h"
#include "MyType.h"

u16 fac_ms;
u8 fac_us;

__IO u16 ntime;	
							    
void delay_ms(u16 nms)
{
   	  SysTick->LOAD = (u32)fac_ms*nms-1;
	  SysTick->VAL = 1;
	  SysTick->CTRL |= BIT(0);//SysTick enable
	  while(!(SysTick->CTRL&(1<<16)));
	  SysTick->CTRL &=~BIT(0);//close SysTick
}

void Delay_Init(u8 SYSCLK)
{
     SysTick->CTRL &=~BIT(2);
	 SysTick->CTRL &=~BIT(1);
	 fac_us = SYSCLK/8;
	 fac_ms = (u16)fac_us*1000;	 
}

		    								   
void delay_us(u32 nus)
{		
	  SysTick->LOAD = (u32)fac_us*nus-1;
	  SysTick->VAL = 1;
	  SysTick->CTRL |= BIT(0);
	  while(!(SysTick->CTRL&(1<<16)));
	  SysTick->CTRL &=~BIT(0);
}


void Delay(u32 count)
{
	while(count--);
}
