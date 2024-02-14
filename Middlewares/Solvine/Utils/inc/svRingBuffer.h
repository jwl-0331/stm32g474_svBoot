////////////////////////////////////////////////////////////////////////////////
// Author : Solvine
// Creation Date : 200211
// File : svRingBuffer.h
// Desc. : Ring Buffer helper functions
//
////////////////////////////////////////////////////////////////////////////////

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SVRINGBUFFER_H__
#define __SVRINGBUFFER_H__

// Definitions
#define svRINGBUFF_DEF(_name,_size)         \
    unsigned char _name##_dataSpace[_size];     \
    svRingBuffer_t _name = {              \
        .buf  = _name##_dataSpace,    \
        .head = _name##_dataSpace,    \
        .tail = _name##_dataSpace,    \
        .bufend = _name##_dataSpace + _size,	\
        .size = _size,                 \
        .fOverflow = 0,            \
        .fTurnAround = 0,            \
    }

typedef struct {
	unsigned char  *buf;
	unsigned char  *head;
	unsigned char  *tail;
	unsigned char  *bufend;
	unsigned int   size;
	int 	 fOverflow;
	int 	 fTurnAround;
} svRingBuffer_t;

// Function Declarations

unsigned int svRingBuffer_GetSize(const svRingBuffer_t *rb);
unsigned int svRingBuffer_GetCount(const svRingBuffer_t *rb);
const unsigned char* svRingBuffer_Tail(const svRingBuffer_t *rb);
const unsigned char* svRingBuffer_Head(const svRingBuffer_t *rb);
const unsigned char* svRingBuffer_Buf(const svRingBuffer_t *rb);
void svRingBuffer_Reset(svRingBuffer_t *rb);
unsigned int svRingBuffer_Capacity(const svRingBuffer_t *rb);
unsigned int svRingBuffer_FreeBytes(const svRingBuffer_t *rb);
int svRingBuffer_IsFull(const svRingBuffer_t *rb);
int svRingBuffer_IsEmpty(const svRingBuffer_t *rb);
int svRingBuffer_IsOverflow(const svRingBuffer_t *rb);
void svRingBuffer_ClearOverflow(svRingBuffer_t *rb);
int inline svRingBuffer_Push(svRingBuffer_t *rb, unsigned char data);
unsigned char inline svRingBuffer_Pop(svRingBuffer_t *rb);

#endif // __SVRINGBUFFER_H__
