/*
 * ringbuffer.
 * ring buffer structures
 */

#pragma once

#include <stdint.h>

#define RING_CHAR_BUF_SIZE 256

typedef struct{
	uint8_t putAt;
	uint8_t getAt;
	char buffer[RING_CHAR_BUF_SIZE];
}CharRingBuffer_t;

//RingBuffer_Put: insert element to buffer
//parameter: element to insert
void RingBuffer_Put(volatile CharRingBuffer_t* buffer, char element);

//RingBuffer_PutNoBlock: try inserting element to buffer, does not block
//parameter: element to insert
void RingBuffer_PutNoBlock(volatile CharRingBuffer_t* buffer, char element);

//RingBuffer_Get: read oldest element from buffer
//return: oldest element from buffer
char RingBuffer_Get(volatile CharRingBuffer_t* buffer);

//RingBuffer_GetNoBlock: try reading oldest element from buffer, does not block
//return: oldest element from buffer
char RingBuffer_GetNoBlock(volatile CharRingBuffer_t* buffer);

//RingBuffer_IsEmpty: determine if buffer is empty
//return: false(0), true(!=0)
int RingBuffer_IsEmpty(volatile CharRingBuffer_t* buffer);
