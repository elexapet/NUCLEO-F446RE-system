/*
 * Embedded application:
 * -Final Project (scheduler)-
 * Laboratory assignment #8
 *
 *
 * main.c
 *
 *  Created on: Jan 16, 2017
 *      Author: Elexa
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "uart.h"
#include "delay.h"
#include "lcd.h"
#include "keypad.h"
#include "led.h"
#include "music.h"
#include "systick.h"
#include "scheduler.h"
#include "tempsensor.h"

#define BAUD_RATE 19200

#define MenuItemSize 7
const char* MenuItem[] = {
		"switch led bar lights off",
		"select internal temperature sensor",
		"select external temperature sensor",
		"play a song",
		"pause song",
		"moving LEDs on(1)/off(0)",
		"help"};

const char* HelpText = "\nType in menu selection followed by necessary information.\n";


void musicPlayTask(void* args)
{
	const MusicNote_t* song = ImperialMarch;
	unsigned count = 1;

	if (args != 0)
	{
		song = *(MusicNote_t**)args;
		count = *(unsigned*)(args+4);
	}
	Music_Play(song, count);
}

void blinkLedTask(void* args)
{
	unsigned number = 0b111;
	unsigned dir = 0;

	while(1){
		LedBar_Light(number);
		//knight light logic
		if (dir) number >>= 1;
		else number <<= 1;
		if (number == 0b1110000000 || number == 0b111) dir = !dir;
		System_DelayMs(60);
	}
}

void calculatorAppTask()
{
	char key;
	char buffer[10];
	int size = 0;
	char op = 0;
	int nums[2] = {0,0};
	int cursorPosition = 0;

	Lcd_Clear();
	Lcd_PrintNumber(0);
	Lcd_SetCursor(0,0);

	while(1){
		key = Key_GetChar();
		//read operands
		if (key >= '0' && key <= '9'){
			if(size >= 9) continue;
			buffer[size++] = key;
			Lcd_SetCursor(0, cursorPosition++);
			Lcd_PrintChar(key);
		}
		//get operation
		else if(key >= 'A' && key <= 'D'){
			buffer[size] = 0;
			nums[0] = atoi(buffer);
			size = 0;
			op = key;

			Lcd_Clear();
			Lcd_PrintNumber(0);
			Lcd_SetCursor(0,0);
			cursorPosition = 0;
		}
		//get result
		else if (key == '#'){
			buffer[size] = 0;
			nums[1] = atoi(buffer);
			Lcd_SetCursor(1,0);
			switch(op){
			case 'A':
				Lcd_PrintNumber(nums[0]+nums[1]);
				break;
			case 'B':
				Lcd_PrintNumber(nums[0]-nums[1]);
				break;
			case 'C':
				Lcd_PrintNumber(nums[0]*nums[1]);
				break;
			case 'D':
				Lcd_PrintNumber(nums[0]/nums[1]);
				break;
			}
			size = 0;
			op = 0;
			Lcd_SetCursor(0,0);
			Lcd_PrintString("0          ");
			Lcd_SetCursor(0,0);
			cursorPosition = 0;
		}
		//clear
		else{
			op = 0;
			size = 0;
			Lcd_Clear();
			Lcd_PrintNumber(0);
			Lcd_SetCursor(0,0);
			cursorPosition = 0;
		}
	}
}

void printMenu()
{
	for(int i = 0; i < MenuItemSize; i++)
		printf("%d: %s\n", i, MenuItem[i]);
	putc('\n', stdout);
}

void consoleAppTask()
{
	char input[25];
	char* token;
	uint command;
	uint cmdArg;

	//create knight rider lights task
	int lightsTask = Sched_CreateTask(500, blinkLedTask, NULL, 5);
	Sched_SuspendTask(lightsTask);
	//create music playing task
	void* musicTaskArgs[] = {ImperialMarch, 0};
	int musicTask = Sched_CreateTask(1000, musicPlayTask, musicTaskArgs, 5);
	Sched_SuspendTask(musicTask);

	while(1)
	{
		printMenu();
		//read input
		fgets(input, 25, stdin);
		token = strpbrk(input, "0123456");
		//parse arguments
		uint recArgs = sscanf(input, "%u%u", &command, &cmdArg);

		switch (token ? *token : -1)
		{
			case '0':
				LedBar_Light(0);
				printf("led bar -> lights off\n");
				break;
			case '1':
				Temp_SelectChip();
				printf("internal sensor selected\n");
				break;
			case '2':
				Temp_SelectExternal();
				printf("external sensor selected\n");
				break;
			case '3':
				printf("playing imperial march...\n");
				Sched_ResumeTask(musicTask);
				break;
			case '4':
				printf("music paused\n");
				Sched_SuspendTask(musicTask);
				break;
			case '5':
				if (recArgs < 2) goto RecError;
				if (cmdArg) Sched_ResumeTask(lightsTask);
				else Sched_SuspendTask(lightsTask);
				printf("knight lights pause/resumed\n");
				break;
			case '6':
				printf("%s\n", HelpText);
				break;
			default:
RecError:		printf("Invalid selection/arguments\n");
				break;
		}
		putc('\n', stdout);
	}
}

void tempDisplayTask()
{
	unsigned average = Temp_LastSample;
	char text[10];

	while(1)
	{
		//running average
		average = (Temp_LastSample + average) / 2;
		//convert
		int temp = Temp_CelsiusFromRaw(average);
		//separate whole and fraction part
		int whole = temp/10;
		unsigned fraction = temp - (whole * 10);
		//convert to string
		int ret = snprintf(text, 10, "%d.%uC", whole, fraction);
		//print on LCD
		Lcd_SetCursor(1, LCD_COLS - ret);
		Lcd_PrintString(text);
		Lcd_SetCursor(0, 0);
		//wait some time
		System_DelayMs(1000);
	}
}

int main()
{
	Key_Init();
	LedBar_Init();
	Speaker_Init();
	USART2_init(BAUD_RATE);
	Sched_Init(5, 10);
	SysTick_Init();
	//these need delays working:
	Lcd_Init();
	Temp_Init(1);

	//calculator task
	Sched_CreateTask(1000, calculatorAppTask, NULL, 5);

	//temperature monitor task
	Sched_CreateTask(500, tempDisplayTask, NULL, 5);

	//main task runs console
	consoleAppTask();

	return 0;
}

