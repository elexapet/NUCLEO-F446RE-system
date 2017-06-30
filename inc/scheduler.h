/*
 * scheduler.h
 *
 * round-robin task scheduler
 */

#pragma once

#include "_STM32F446.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

//Sched_Init: initialize scheduler and main task
void Sched_Init(unsigned numberOfTasks, unsigned mainTimeSlice);

//Sched_CreateTask: create a new task
//parameter: stack size
//parameter: function pointer to entry point
//parameter: pointer to arguments object which is passed to entry point
//parameter: time slice
//return: task id
int Sched_CreateTask(unsigned stackSize, void(*entryPoint)(void*), void* args, unsigned timeSlice);

//Sched_SuspendTask: suspend a task
//parameter: task id
void Sched_SuspendTask(unsigned id);

//Sched_SuspendTask: resume a task
//parameter: task id
void Sched_ResumeTask(unsigned id);

//Sched_TaskSleep: current task will not be scheduled for given duration
//parameter: duration in miliseconds to sleep
void Sched_TaskSleep(unsigned duration)

//Sched_Tick: keep scheduler running
//needs to be called on every tick
void Sched_Tick();
