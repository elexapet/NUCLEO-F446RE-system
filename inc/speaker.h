/*
 * speaker.h
 * on-board speaker driver
 */

#pragma once

#include "_STM32F446.h"

//Speaker_Init: initialize peripherals for use with API functions
void Speaker_Init();

//Speaker_Tone: output given tone
//parameter: frequency (>=123Hz)
void Speaker_Tone(unsigned toneFrequency);

//Speaker_Mute: disable speaker output
void Speaker_Mute();
