/*
 * tempsensor.h
 * temperature sensor driver
 */

#pragma once

#include "delay.h"

volatile uint16_t ADC1_Result;

//Latest raw sample from selected temperature sensor
#define Temp_LastSample ADC1_Result

//Temp_Init: initialize temperature sensor and ADC
//default sensor is internal
//parameter: enable sample collection to be interrupt driven
void Temp_Init(int enableInterrupt);

//Temp_SelectChip: select internal sensor
void Temp_SelectChip();

//Temp_SelectExternal: select external sensor
void Temp_SelectExternal();

//Temp_MeasureRaw: read sensor raw value
//blocks until sample ready
//use only when sampling is not interrupt driven
uint16_t Temp_MeasureRaw();

//Temp_FromRaw: convert temperature from raw value to Celsius
int Temp_CelsiusFromRaw(unsigned rawValue);

//Temp_FahrenheitFromRaw: convert temperature from raw value to Fahrenheit
int Temp_FahrenheitFromRaw(unsigned rawValue);
