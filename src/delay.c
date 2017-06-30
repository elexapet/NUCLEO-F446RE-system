#include "delay.h"

static volatile SYSTICK * const pSysTick = (SYSTICK *)STK_BASE;

void System_DelayMs(unsigned theDelay)
{
	uint32_t start = System_GetTicks();
	uint32_t stop = start + theDelay;

	//in case of underflow
	if (stop < start)
	{
		while(System_GetTicks() > stop);
	}
	while(System_GetTicks() < stop);
}

void System_DelayUs(unsigned theDelay)
{
	//call System_DelayMs if the delay is too large
	if (theDelay > 1000) System_DelayMs(theDelay/1000);

	uint32_t start = pSysTick->VAL;
	int32_t stop = start - theDelay * 16;

	if (stop < 0) stop += 16000;

	//in case of underflow
	if (stop > start)
	{
		while(pSysTick->VAL < stop);
	}
	while(pSysTick->VAL > stop);
}
