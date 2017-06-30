/*
 * uart.h
 *  serial console driver
 *
 *  Created on: Jan 20, 2017
 *      Author: elexap
 */

#pragma once

#include "_STM32F446.h"
#include "ringbuffer.h"
#include <stdint.h>
#include <stdio.h>

//USART2_init: initialize USART2 peripheral for serial port (8N1)
//parameter: desired baud rate
void USART2_init(unsigned baudRate);

//USART2_get: read character from serial console
char USART2_get();
//USART2_put: write character to serial console
void USART2_put(char data);
