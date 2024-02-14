////////////////////////////////////////////////////////////////////////////////
// Author : Solvine
// Creation Date : 200211
// File : svOvRingBuffer.h
// Desc. : Overwrittable Ring Buffer helper functions
//
////////////////////////////////////////////////////////////////////////////////

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SVOVRINGBUFFER_H__
#define __SVOVRINGBUFFER_H__

// Definitions
typedef unsigned int	OvRB_t;			// Change this for proper unit !!

#define svOvRINGBUFF_DEF(_name,_size)  \
  static OvRB_t _name##_dataSpace[_size];   \
  svOvRingBuffer_t _name = {       \
      .buf  = _name##_dataSpace,   \
      .current = 0,								 \
      .count = 0,           			 \
      .sum = 0,										 \
      .size = _size,               \
  }

typedef struct
{
	OvRB_t *buf;
	unsigned int current;		// Current Data Pointer to (over)write
	unsigned int count;			// Current total number of Data
	unsigned int size;			// Size of this buffer
	OvRB_t	sum;						// Sum for Moving average
} svOvRingBuffer_t;

// Function Declarations

void svOvRingBuffer_Init(svOvRingBuffer_t* pOvRB);
unsigned int svOvRingBuffer_GetSize(svOvRingBuffer_t* pOvRB);
unsigned int svOvRingBuffer_GetCount(svOvRingBuffer_t* pOvRB);
unsigned int svOvRingBuffer_AddData(svOvRingBuffer_t* pOvRB, OvRB_t data);
OvRB_t svOvRingBuffer_GetData(svOvRingBuffer_t* pOvRB, int nbNotFirst);
OvRB_t svOvRingBuffer_GetMovingAverage(svOvRingBuffer_t* pOvRB);

#endif // __SVOVRINGBUFFER_H__
