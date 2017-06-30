#include "speaker.h"

static volatile TIM2_5 * const pTimer3 = (TIM2_5 *)TIM3_BASE;

void Speaker_Init()
{
	//enable GPIOB
	*RCC_AHB1ENR |= GPIOB_EN;

	//enable TIM3
	*RCC_APB1ENR |= TIM3_EN;

	//setup PB4 for AF2
	GpioB->AFRL = (GpioB->AFRL & ~AFRL4_MASK) | AFRL4_AF2;
	GpioB->MODER.MOD4 = MODERx_AFM;

	//set TIM3_ARR value
	pTimer3->ARR = 8000;
	//set TIM3_CCR1 value (1KHZ default)
	pTimer3->CCR1 = 8000;

	//set to toggle on match in TIM3_CCMR1
	pTimer3->CCMR1 &= ~OC1M_MASK;
	pTimer3->CCMR1 |= OC1M_TOGGLE;

	//enable output in TIM3_CCER
	pTimer3->CCER = (pTimer3->CCER & ~(CCER_CC1P|CCER_CC1NP)) | CCER_CC1E;
}

void Speaker_Tone(unsigned freq)
{
	//get length of half-period in ticks
	unsigned ticks = 8000000/freq;

	//set TIM3_ARR value
	pTimer3->ARR = ticks;

	//set TIM3_CCR1 value
	pTimer3->CCR1 = ticks;

	//enable counter in TIM3_CR1
	pTimer3->CR1.CEN = 1;
}

void Speaker_Mute()
{
	//disable counter in TIM3_CR1 (better to disable just channel 1)
	pTimer3->CR1.CEN = 0;
}
