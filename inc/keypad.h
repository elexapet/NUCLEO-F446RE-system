/*
 * keypad.h
 *
 * keypad driver
 */

#pragma once

#include "_STM32F446.h"
#include "delay.h"

//key_init initializes I/O pins for keypad operation
void Key_Init();

//key_getkey_noblock does not wait for key press
//Returns key code of pressed key, when no key is pressed returns zero
unsigned char Key_GetKeyNoBlock();

//key_getkey blocks until key is pressed and then released
//returns key code of pressed key
unsigned char Key_GetKey();

//key_getchar blocks until key is pressed
//returns ASCII code of pressed key
char Key_GetChar();
