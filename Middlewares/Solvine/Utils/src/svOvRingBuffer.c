////////////////////////////////////////////////////////////////////////////////
// Author : Solvine
// Creation Date : 200211
// File : svOvRingBuffer.c
// Desc. : Overwrittable Ring Buffer helper functions
//
////////////////////////////////////////////////////////////////////////////////
#include "BSPConfig.h"			// for Optimize
#include "svOvRingBuffer.h"
// Definitions
// Macros
// Global variables
// Static Variables
// extern variables

// Functions
void svOvRingBuffer_Init(svOvRingBuffer_t* pOvRB)
{
	if (pOvRB) {
		pOvRB->count = 0;
		pOvRB->current = 0;
		pOvRB->sum = 0;
		for (int i=0;i<pOvRB->size;i++) {
			pOvRB->buf[i] = 0;
		}
	}
}

unsigned int svOvRingBuffer_GetSize(svOvRingBuffer_t* pOvRB)
{
	if (pOvRB)
		return pOvRB->size;
	return 0;
}

unsigned int svOvRingBuffer_GetCount(svOvRingBuffer_t* pOvRB)
{
	if (pOvRB)
		return pOvRB->count;
	return 0;
}

// Getting moving average
OvRB_t svOvRingBuffer_GetMovingAverage(svOvRingBuffer_t* pOvRB)
{
	if (pOvRB && (pOvRB->count > 0)) {
		return (pOvRB->sum)/(pOvRB->count);
	}
	return 0;
}

// 
unsigned int svOvRingBuffer_AddData(svOvRingBuffer_t* pOvRB, OvRB_t data)
{
	// for speed, skip integrity check !!
	pOvRB->sum += data;
	pOvRB->sum -= pOvRB->buf[pOvRB->current];

	pOvRB->buf[pOvRB->current++] = data;
	if (pOvRB->count < pOvRB->size) {
		pOvRB->count++;
	}
	// Wrap around
	if (pOvRB->current >= pOvRB->size) {
		pOvRB->current = 0;
	}
	return pOvRB->count;
}

OvRB_t svOvRingBuffer_GetData(svOvRingBuffer_t* pOvRB, int nbNotFirst)
{
	// for speed, skip integrity check !!
	static int snPoint = 0;
	OvRB_t retData;
	if (0 == nbNotFirst) {	// means First !!
		if (pOvRB->current == 0)
			snPoint = pOvRB->size - 1;
		else			
			snPoint = pOvRB->current - 1;
	}
	retData = pOvRB->buf[snPoint];
	snPoint--;
	if (snPoint < 0)
		snPoint = pOvRB->size - 1;
	return retData;
}

