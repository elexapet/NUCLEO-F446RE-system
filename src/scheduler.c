#include <scheduler.h>

//forward declaration
static void Sched_TaskFinish();

/*
 * Default stack content
 *
 * PSR = default 0x01000000
 * PC <= entry point
 * LR = default 0xFFFFFFFF
 * R12
 * R3-R0
 * ISR LR = 0xFFFFFFF9
 * R11-R4
 */
#define DefStackFrameSize 17
static const void* DefStackFrame[]
	= {0, 0, 0, 0, 0, 0, 0, 0, 0xFFFFFFF9, 0, 0, 0, 0, 0, Sched_TaskFinish, 0, 0x01000000};

//----------------------------------
//type definitions
//----------------------------------
typedef enum{
	Ready,
	Active,
	Blocked,
	Finished
}TaskState_t;

typedef struct{
	void** StackPointer;
	void** StackAllocPointer;
	TaskState_t State;
	unsigned TicksStarting;
	unsigned TicksRemaining;
	unsigned TicksBlocked;
}TaskInfo_t;
//----------------------------------

static unsigned TaskListSize = 0;
static TaskInfo_t* TaskList = NULL;
static unsigned NextId = 1;
static unsigned CurrentTask = 0;
static unsigned NextTask = 0;

void Sched_Init(unsigned numberOfTasks, unsigned mainTimeSlice)
{
	//allocate task list
	TaskListSize = numberOfTasks;
	TaskList = calloc(TaskListSize, sizeof(TaskInfo_t));
	//main task update status
	TaskList[0].State = Active;
	TaskList[0].TicksRemaining = mainTimeSlice;
	TaskList[0].TicksStarting = mainTimeSlice;
}

int Sched_CreateTask(unsigned stackSize, void(*entryPoint)(void*), void* args, unsigned timeSlice)
{
	assert(TaskList != NULL);
	assert(NextId < TaskListSize);

	DISABLE_INTERRUPT

	//free previously allocated stack
	if (TaskList[NextId].StackAllocPointer != NULL) free(TaskList[NextId].StackAllocPointer);
	//allocate stack
	TaskList[NextId].StackAllocPointer = malloc(stackSize * sizeof(void*));
	TaskList[NextId].StackPointer = TaskList[NextId].StackAllocPointer + stackSize;

	//fill task's stack with default values
	memcpy(TaskList[NextId].StackPointer - DefStackFrameSize, DefStackFrame, DefStackFrameSize*sizeof(void*));
	//assign task's entry point
	*(TaskList[NextId].StackPointer - 2) = entryPoint;
	*(TaskList[NextId].StackPointer - 8) = args;
	//move stack pointer to the top
	TaskList[NextId].StackPointer -= DefStackFrameSize;

	//update task status
	TaskList[NextId].State = Active;
	TaskList[NextId].TicksStarting = timeSlice;
	TaskList[NextId].TicksRemaining = 0;

	ENABLE_INTERRUPT

	//return task id
	return NextId++;
}

void Sched_SuspendTask(unsigned id)
{
	assert(TaskList != NULL);
	assert(id < TaskListSize);
	//change state
	TaskList[id].State = Ready;
}

void Sched_ResumeTask(unsigned id)
{
	assert(TaskList != NULL);
	assert(id < TaskListSize);
	//change state
	TaskList[id].State = Active;
}

void Sched_TaskSleep(unsigned duration)
{
	assert(TaskList != NULL);

	//change to blocked
	TaskList[CurrentTask].State = Blocked;
	TaskList[CurrentTask].TicksBlocked = duration;
	TaskList[CurrentTask].TicksRemaining = 1;
	//switch to next task
	for (NextTask = CurrentTask + 1; NextTask != CurrentTask; ++NextTask)
	{
		if (NextTask >= NextId) NextTask -= NextId;
		if (TaskList[NextTask].State == Active) break;
	}
	//assign time slice
	TaskList[NextTask].TicksRemaining = TaskList[NextTask].TicksStarting;
	//trigger context switch interrupt
	((ICSR_t*)ICSR_BASEADDR)->PENDSVSET = 1;
}

static void Sched_TaskFinish()
{
	//change task's state to finished and switch to next active task
	TaskList[CurrentTask].State = Finished;
	for (NextTask = CurrentTask + 1; NextTask != CurrentTask; ++NextTask)
	{
		if (NextTask >= NextId) NextTask -= NextId;
		if (TaskList[NextTask].State == Active) break;
	}
	//assign time slice
	TaskList[NextTask].TicksRemaining = TaskList[NextTask].TicksStarting;
	//trigger context switch interrupt
	((ICSR_t*)ICSR_BASEADDR)->PENDSVSET = 1;
}

void Sched_Tick()
{
	//check if scheduler not present
	assert(TaskList != NULL);
	
	//decrement ticks for blocked tasks
	for (int i = 0; i < NextId; ++i)
	{
		if (TaskList[i].State == Blocked)
		{
			--TaskList[i].TicksBlocked;
			if (TaskList[i].TicksBlocked == 0) TaskList[i].State == Running;
		}
	}

	//do nothing if task has ticks left
	if (--TaskList[CurrentTask].TicksRemaining > 0) return;
	//find task in ready state
	for (NextTask = CurrentTask + 1; ; ++NextTask)
	{
		if (NextTask >= NextId) NextTask -= NextId;
		if (CurrentTask == NextTask) break;
		if (TaskList[NextTask].State == Active) break;
	}
	//assign time slice
	TaskList[NextTask].TicksRemaining = TaskList[NextTask].TicksStarting;
	//trigger context switch interrupt
	((ICSR_t*)ICSR_BASEADDR)->PENDSVSET = 1;
}

//context switcher
__attribute__((naked, isr))
void PendSV_Handler(void)
{
	register void** stackPointer asm("r13");
	//only r4-r11 are not auto stacked
	asm volatile("push {r4-r11, lr}");

	//switch stack pointers
	TaskList[CurrentTask].StackPointer = stackPointer;
	stackPointer = TaskList[NextTask].StackPointer;

	//update task information
	CurrentTask = NextTask;

	//return from isr to next task
	asm volatile("pop {r4-r11, lr}");
	asm volatile("bx lr");
}

//supervisor call
__attribute__((isr))
void SVCall_Handler(void)
{

}
