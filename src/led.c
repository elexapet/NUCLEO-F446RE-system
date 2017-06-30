#include "led.h"

void LedBar_Init()
{
	//enable GPIOA and GPIOB
	*RCC_AHB1ENR |= GPIOA_EN | GPIOB_EN;

	//set PA7-PA11 for output
	GpioA->MODER.val = (GpioA->MODER.val & ~0x00AA8000) | 0x00554000;

	//set PB8-10, PB12-13 for output
	GpioB->MODER.val = (GpioB->MODER.val & ~0x0A2A0000) | 0x05150000;

	//turn off all LEDs
	GpioA->ODR.val &= ~0x0F80;
	GpioB->ODR.val &= ~0x3700;
}

void LedBar_Light(unsigned withNumber)
{
	//discard unused bits
	withNumber &= 0x3FF;

	//extract lower 5 bits
	GpioA->ODR.val = (~0x0F80 & GpioA->ODR.val) | (withNumber & 0x01F) << 7;
	//extract upper 5 bits (also skip PB11)
	GpioB->ODR.val = (~0x3700 & GpioB->ODR.val) | (withNumber & 0x0E0) << (8-5) | (withNumber & 0x300) << (13-9);
}
