/*
 * delay.h
 * system delays
 */

#pragma once

#include "_STM32F446.h"
#include "systick.h"

//System_DelayMs: delay for given time
void System_DelayMs(unsigned theDelay);

//System_DelayUs: delay for given time
//for delay longer than 1000us call System_DelayMs
void System_DelayUs(unsigned theDelay);
