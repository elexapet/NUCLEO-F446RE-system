#include "systick.h"

static volatile uint32_t Ticks = 0;

void SysTick_Init()
{
	volatile SYSTICK * const pSysTick = (SYSTICK *)STK_BASE;
	//tick every millisecond
	pSysTick->LOAD = (F_CPU / 1000) - 1;
	pSysTick->VAL = 0;
	//enable timer, disable divider, enable IRQ on reload
	pSysTick->CTRL = 0b111;
}

uint32_t System_GetTicks()
{
	return Ticks;
}

__attribute__((isr))
void SysTick_Handler(void)
{
	Ticks++;
	//call scheduler as last
	Sched_Tick();
}
