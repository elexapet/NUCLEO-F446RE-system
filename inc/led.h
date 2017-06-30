/*
 * led.h
 *
 * LED bar driver
 */

#pragma once

#include "_STM32F446.h"
#include <stdint.h>

void LedBar_Init();
void LedBar_Light(unsigned withNumber);
