/*
 * lcd.h
 * on-board LCD API
 */

#pragma once

#include "_STM32F446.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "delay.h"

#define LCD_COLS 16
#define LCD_ROWS 2

//Lcd_Init: must be called before any other function
void Lcd_Init();

//Lcd_Clear: clear display
void Lcd_Clear();

//Lcd_Home: return cursor to default position [0,0]
void Lcd_Home();

//Lcd_SetCursor: move cursor to selected position
//parameters: row number <0,1>, column number <0,39>
void Lcd_SetCursor(unsigned lineNumber, unsigned columnNumber);

//Lcd_PrintString: print zero terminated ASCII string
//parameter: address of string
//return: number of characters written to LCD
size_t Lcd_PrintString(const char * stringToPrint);

//Lcd_PrintString: print number in range <0,9999> to LCD
//parameter: number to print
//return: number of characters written to LCD
size_t Lcd_PrintNumber(int numberToPrint);

//Lcd_PrintChar: print character on the LCD at current cursor position
//parameter: character to print
void Lcd_PrintChar(char charToPrint);
