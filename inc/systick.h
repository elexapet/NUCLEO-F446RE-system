/*
 * systick.h
 * system timer functions
 */

#pragma once

#include <stdint.h>
#include "_STM32F446.h"
#include "scheduler.h"

//initialize system timer
void SysTick_Init();

//return: current system ticks value
uint32_t System_GetTicks();
