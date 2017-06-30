#include <_STM32F446.h>
#include "keypad.h"

//keycode to key char translation map
static const char KEYMAP[4][4] = {{'1','2','3','A'},
							{'4','5','6','B'},
							{'7','8','9','C'},
							{'*','0','#','D'}};

//PC0-3 = columns | PC4-7 = rows
void Key_Init()
{
	//enable GPIOC
	*RCC_AHB1ENR |= GPIOC_EN;

	//set PC0-3 as output and PC4-7 as input
	GpioC->MODER.MOD0 = MODERx_OUT;
	GpioC->MODER.MOD1 = MODERx_OUT;
	GpioC->MODER.MOD2 = MODERx_OUT;
	GpioC->MODER.MOD3 = MODERx_OUT;
	GpioC->MODER.MOD4 = MODERx_IN;
	GpioC->MODER.MOD5 = MODERx_IN;
	GpioC->MODER.MOD6 = MODERx_IN;
	GpioC->MODER.MOD7 = MODERx_IN;

	//setup pull-ups on inputs (PC4-7)
	GpioC->PUPDR.val = (~0xFF00 & GpioC->PUPDR.val) | 0x5500;
}

unsigned char Key_GetKeyNoBlock()
{
	unsigned char colSelect = 0x8;
	unsigned char rowsVal;

	while(colSelect){
		//write zero to selected column
		GpioC->BSRR.val = (0xF000F | GpioC->BSRR.val) & ~colSelect;
		//give column time to get low (discharge)
		//this avoids floating input
		__asm__ volatile("nop\n\t"
				"nop\n\t"
				"nop\n\t"
				"nop\n\t"
				"nop\n\t"
				"nop\n\t");

		rowsVal = GpioC->IDR.val & 0xF0;
		//invert values for all rows
		rowsVal ^= 0xF0;
		if(rowsVal) return rowsVal | colSelect;
		colSelect >>= 1;
	}

	return 0;
}

unsigned char key_getkey()
{
	unsigned char keyRaw;
	//pool Key_GetKeyNoBlock until key press is registered
	do{
		keyRaw = Key_GetKeyNoBlock();
		System_DelayMs(7);
	}while(!keyRaw);

	//return after key is released
	while(Key_GetKeyNoBlock());

	return keyRaw;
}

char Key_GetChar()
{
	unsigned char c = 0;
	unsigned char r = 0;
	unsigned char keyRaw = key_getkey();
	unsigned char nibble = keyRaw >> 4;
	//get row number
	while (nibble >>= 1) ++r;

	nibble = keyRaw & 0xF;
	//get column number
	while (nibble >>= 1) ++c;

	//map to ASCII
	return KEYMAP[r][c];
}
