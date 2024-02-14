////////////////////////////////////////////////////////////////////////////////
// Author : Solvine
// Creation Date : 200211
// File : svRingBuffer.c
// Desc. : Ring Buffer helper functions
//
////////////////////////////////////////////////////////////////////////////////
#include "svRingBuffer.h"

unsigned int svRingBuffer_GetSize(const svRingBuffer_t *rb)
{
	return rb->size;
}

unsigned int svRingBuffer_GetCount(const svRingBuffer_t *rb)
{
	unsigned char *h, *t;
	// use temporal value to avoid corruption in between context-switching / Interrupt 
	h = rb->head;
	t = rb->tail;
	if (h >= t)
	  return (h - t);
	else
	  return (unsigned int)(rb->bufend - t + h - rb->buf);
}

void svRingBuffer_Reset(svRingBuffer_t *rb)
{
	rb->head = rb->tail = rb->buf;
	rb->fOverflow = 0;		// Clear Flag
	rb->fTurnAround = 0;	// Clear Flag
}

unsigned int svRingBuffer_Capacity(const svRingBuffer_t *rb)
{
  return svRingBuffer_GetSize(rb) - 1;
}

unsigned int svRingBuffer_FreeBytes(const svRingBuffer_t *rb)
{
	unsigned char *h, *t;
	// use temporal value to avoid corruption in between context-switching / Interrupt 
	h = rb->head;
	t = rb->tail;
  if (h >= t)
    return svRingBuffer_Capacity(rb) - (h - t);
  else
    return t - h - 1;
}

int svRingBuffer_IsFull(const svRingBuffer_t *rb)
{
  return svRingBuffer_FreeBytes(rb) == 0;
}


int svRingBuffer_IsEmpty(const svRingBuffer_t *rb)
{
  return svRingBuffer_FreeBytes(rb) == svRingBuffer_Capacity(rb);
}

const unsigned char* svRingBuffer_Tail(const svRingBuffer_t *rb)
{
  return rb->tail;
}

const unsigned char* svRingBuffer_Head(const svRingBuffer_t *rb)
{
  return rb->head;
}

const unsigned char* svRingBuffer_Buf(const svRingBuffer_t *rb)
{
  return rb->buf;
}

int svRingBuffer_IsOverflow(const svRingBuffer_t *rb)
{
	return rb->fOverflow;
}

int svRingBuffer_IsTurnAround(const svRingBuffer_t *rb)
{
	return rb->fTurnAround;
}

void svRingBuffer_ClearOverflow(svRingBuffer_t *rb)
{
	rb->fOverflow = 0;
}

int svRingBuffer_Push(svRingBuffer_t *rb, unsigned char data)
{
	int nRet;
	nRet = 0;
	*(rb->head) = data;
	rb->head++;
	if (rb->head == rb->bufend) {	// Wrap around
	  rb->head = rb->buf;
		rb->fTurnAround = 1;
		nRet = -1;
		// return -1;	// -- hellosmk 210512 to check overflow
	}
	if (rb->head == rb->tail) {		// Over flowed
	  rb->fOverflow = 1;
		nRet = -2;
	}
	return nRet;
}

unsigned char svRingBuffer_Pop(svRingBuffer_t *rb)
{
	unsigned char byData;
	byData = *(rb->tail);
	rb->tail++;
	if (rb->tail == rb->bufend)	// Wrap around
	  rb->tail = rb->buf;
	return byData;
}



