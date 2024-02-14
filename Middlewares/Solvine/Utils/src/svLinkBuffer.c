////////////////////////////////////////////////////////////////////////////////
// Author : Solvine
// Creation Date : 210315
// File : svLinkBuffer.c
// Desc. : Ring Buffer helper functions
//
////////////////////////////////////////////////////////////////////////////////
#include "svLinkBuffer.h"

unsigned int svLinkBuffer_GetSize(const svLinkBuffer_t *lb)
{
	return lb->size;
}

unsigned int svLinkBuffer_GetCount(const svLinkBuffer_t *lb)
{
	DataPtr_t* h;
  DataPtr_t* t;
	// use temporal value to avoid corruption in between context-switching / Interrupt 
	h = lb->head;
	t = lb->tail;
	
	if (h >= t)
	  return (unsigned int)(h - t);
	else
	  return (unsigned int)(lb->bufend - t + h - lb->buf);
}

void svLinkBuffer_Reset(svLinkBuffer_t *lb)
{
	lb->head = lb->tail = lb->peek =lb->buf;
	lb->fOverflow = 0;		// Clear Flag
	lb->fTurnAround = 0;	// Clear Flag
}

unsigned int svLinkBuffer_Capacity(const svLinkBuffer_t *lb)
{
  return svLinkBuffer_GetSize(lb) - 1;
}

unsigned int svLinkBuffer_FreeBytes(const svLinkBuffer_t *lb)
{
	DataPtr_t* h;
  DataPtr_t* t;
	// use temporal value to avoid corruption in between context-switching / Interrupt
	h = lb->head;
	t = lb->tail;
	
  if (h >= t)
    return svLinkBuffer_Capacity(lb) - (h - t);
  else
    return t - h - 1;
}

int svLinkBuffer_IsFull(const svLinkBuffer_t *lb)
{
  return svLinkBuffer_FreeBytes(lb) == 0;
}


int svLinkBuffer_IsEmpty(const svLinkBuffer_t *lb)
{
  return svLinkBuffer_FreeBytes(lb) == svLinkBuffer_Capacity(lb);
}

const DataPtr_t* svLinkBuffer_Tail(const svLinkBuffer_t *lb)
{
  return lb->tail;
}

const DataPtr_t* svLinkBuffer_Head(const svLinkBuffer_t *lb)
{
  return lb->head;
}

const DataPtr_t* svLinkBuffer_Buf(const svLinkBuffer_t *lb)
{
  return lb->buf;
}

int svLinkBuffer_IsOverflow(const svLinkBuffer_t *lb)
{
	return lb->fOverflow;
}

int svLinkBuffer_IsTurnAround(const svLinkBuffer_t *lb)
{
	return lb->fTurnAround;
}

void svLinkBuffer_ClearOverflow(svLinkBuffer_t *lb)
{
	lb->fOverflow = 0;
}

int svLinkBuffer_Push(svLinkBuffer_t *lb, DataPtr_t data)
{
	int nRet;
	nRet = 0;
	*(lb->head) = data;
	lb->head++;
	if (lb->head >= lb->bufend) {	// Wrap around
	  lb->head = lb->buf;
		lb->fTurnAround = 1;
		nRet = -1;
		// return -1;	// -- hellosmk 210512 to check overflow
	}
	if (lb->head == lb->tail) {		// Overflowed
	  lb->fOverflow = 1;
		if (lb->overflowcallback) {
			lb->overflowcallback((void*)lb);
		}
		nRet = -2;
	}
	return nRet;
}

DataPtr_t svLinkBuffer_Pop(svLinkBuffer_t *lb)
{
	DataPtr_t lnkData;
	lnkData = *(lb->tail);
	lb->tail++;
	if (lb->tail >= lb->bufend)	// Wrap around
	  lb->tail = lb->buf;
	return lnkData;
}

void svLinkBuffer_Skip(svLinkBuffer_t *lb)
{
	lb->tail++;
	if (lb->tail >= lb->bufend)	// Wrap around
	  lb->tail = lb->buf;
}

DataPtr_t svLinkBuffer_Peek(svLinkBuffer_t *lb, unsigned char bFirst)
{
	DataPtr_t lnkData;
	if (bFirst) {
		lb->peek = lb->tail;
	}
	lnkData = *(lb->peek);
	lb->peek++;
	if (lb->peek >= lb->bufend)	// Wrap around
	  lb->peek = lb->buf;
	return lnkData;
}


