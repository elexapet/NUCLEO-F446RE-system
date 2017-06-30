#include "lcd.h"

#define UNIBBLE_MASK 0xF0
#define LNIBBLE_MASK 0x0F
#define CLR_DATA_NIBBLE 0x0F000000
#define SECOND_ROW_START 0x40
#define CLR_DELAY 2
#define HOME_DELAY 2
#define EXEC_DELAY 40
#define CLR_CMD 0x1
#define HOME_CMD 0x2
#define SET_CURSOR_CMD 0x80

static void Lcd_Latch()
{
	GpioB->BSRR.BS2 = 1; //E high
	System_DelayUs(1);
	GpioB->BSRR.BR2 = 1; //E low
	System_DelayUs(1);
}

static void Lcd_Exec(unsigned char toSend)
{
	//write upper nibble
	GpioC->BSRR.val = ((toSend & UNIBBLE_MASK) << 4) | CLR_DATA_NIBBLE;
	Lcd_Latch();
	//write lower nibble
	GpioC->BSRR.val = ((toSend & LNIBBLE_MASK) << 8) | CLR_DATA_NIBBLE;
	Lcd_Latch();
	System_DelayUs(EXEC_DELAY);
}

static void Lcd_Cmd(unsigned char cmdToSend)
{
	GpioB->BSRR.BR0 = 1; //low
	GpioB->BSRR.BR1 = 1; //low

	Lcd_Exec(cmdToSend);
}

static void Lcd_Data(unsigned char dataToSend)
{
	GpioB->BSRR.BS0 = 1; //RS high
	GpioB->BSRR.BR1 = 1; //RW low
	GpioB->BSRR.BS2 = 1; //E high

	Lcd_Exec(dataToSend);
}

void Lcd_Init()
{
	//enable GPIOB and GPIOC
	*RCC_AHB1ENR |= GPIOB_EN | GPIOC_EN;

	//set PB0-2 to output
	GpioB->MODER.MOD0 = MODERx_OUT;
	GpioB->MODER.MOD1 = MODERx_OUT;
	GpioB->MODER.MOD2 = MODERx_OUT;

	//set PC8-11 to output
	GpioC->MODER.MOD8 = MODERx_OUT;
	GpioC->MODER.MOD9 = MODERx_OUT;
	GpioC->MODER.MOD10 = MODERx_OUT;
	GpioC->MODER.MOD11 = MODERx_OUT;

	Lcd_Cmd(0x30); //8-bit mode
	Lcd_Cmd(0x28); //4-bit mode
	Lcd_Clear();
	Lcd_Home();
	Lcd_Cmd(0x06); //entry mode, move right, no shift
	Lcd_Cmd(0x0C); //display on, cursor off

}

void Lcd_Clear()
{
	Lcd_Cmd(CLR_CMD);
	System_DelayMs(CLR_DELAY);
}

void Lcd_Home()
{
	Lcd_Cmd(HOME_CMD);
	System_DelayMs(HOME_DELAY);
}

void Lcd_SetCursor(unsigned lineNumber, unsigned columnNumber)
{
	if (lineNumber > 0) columnNumber += SECOND_ROW_START;
	Lcd_Cmd(columnNumber | SET_CURSOR_CMD);
}

size_t Lcd_PrintString(const char * stringToPrint)
{
	int i;
	for (i = 0; stringToPrint[i]; ++i) {
		Lcd_Data(stringToPrint[i]);
	}
	return i;
}

size_t Lcd_PrintNumber(int numberToPrint)
{
	char numberASCII[11];
	snprintf(numberASCII, 11, "%d", numberToPrint);
	return Lcd_PrintString(numberASCII);
}

void Lcd_PrintChar(char charToPrint)
{
	Lcd_Data(charToPrint);
}
