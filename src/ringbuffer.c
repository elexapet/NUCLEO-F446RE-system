#include "ringbuffer.h"

void RingBuffer_Put(volatile CharRingBuffer_t* this, char element)
{
	while ((uint8_t)(this->putAt - 1) == this->getAt);

	this->buffer[this->putAt++] = element;
}

void RingBuffer_PutNoBlock(volatile CharRingBuffer_t* this, char element)
{
	if ((uint8_t)(this->putAt - 1) == this->getAt) return;

	this->buffer[this->putAt++] = element;
}

char RingBuffer_Get(volatile CharRingBuffer_t* this)
{
	while (this->putAt == this->getAt);

	return this->buffer[this->getAt++];
}

char RingBuffer_GetNoBlock(volatile CharRingBuffer_t* this)
{
	if (this->putAt == this->getAt) return -1;

	return this->buffer[this->getAt++];
}

int RingBuffer_IsEmpty(volatile CharRingBuffer_t* this)
{
	if (this->putAt == this->getAt) return 1;
	return 0;
}
