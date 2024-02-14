////////////////////////////////////////////////////////////////////////////////
// Author : Solvine
// Creation Date : 210315
// File : svLinkBuffer.h
// Desc. : Link Ring Buffer helper functions
//
////////////////////////////////////////////////////////////////////////////////

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SVLINKBUFFER_H__
#define __SVLINKBUFFER_H__

// Definitions
typedef void*			DataPtr_t;			// Change this for proper unit !!

typedef void (*pfnLBOverflowCallback)(void* pLinkBuffer);

// Definitions
#define svLINKBUFF_DEF(_name,_size, _ovfCallback)         \
    DataPtr_t _name##_dataSpace[_size];     \
    svLinkBuffer_t _name = {              \
        .buf  = _name##_dataSpace,    \
        .head = _name##_dataSpace,    \
        .tail = _name##_dataSpace,    \
        .peek = _name##_dataSpace,    \
        .bufend = _name##_dataSpace + _size,	\
        .size = _size,                 \
        .fOverflow = 0,            \
        .fTurnAround = 0,            \
        .overflowcallback = _ovfCallback,			\
    }

typedef struct structSvLinkBuffer_t {
	DataPtr_t  *buf;
	DataPtr_t  *head;
	DataPtr_t  *peek;		// to support svLinkBuffer_Peek
	DataPtr_t  *tail;
	DataPtr_t  *bufend;
	unsigned int   size;
	int 	 fOverflow;
	int 	 fTurnAround;
	pfnLBOverflowCallback	overflowcallback;
} svLinkBuffer_t;

// Function Declarations

unsigned int svLinkBuffer_GetSize(const svLinkBuffer_t *lb);
unsigned int svLinkBuffer_GetCount(const svLinkBuffer_t *lb);
const DataPtr_t* svLinkBuffer_Tail(const svLinkBuffer_t *lb);
const DataPtr_t* svLinkBuffer_Head(const svLinkBuffer_t *lb);
const DataPtr_t* svLinkBuffer_Buf(const svLinkBuffer_t *lb);
void svLinkBuffer_Reset(svLinkBuffer_t *lb);
unsigned int svLinkBuffer_Capacity(const svLinkBuffer_t *lb);
unsigned int svLinkBuffer_FreeBytes(const svLinkBuffer_t *lb);
int svLinkBuffer_IsFull(const svLinkBuffer_t *lb);
int svLinkBuffer_IsEmpty(const svLinkBuffer_t *lb);
int svLinkBuffer_IsOverflow(const svLinkBuffer_t *lb);
void svLinkBuffer_ClearOverflow(svLinkBuffer_t *lb);
int inline svLinkBuffer_Push(svLinkBuffer_t *lb, DataPtr_t data);
DataPtr_t inline svLinkBuffer_Pop(svLinkBuffer_t *lb);
DataPtr_t inline svLinkBuffer_Peek(svLinkBuffer_t *lb, unsigned char bFirst);
void inline svLinkBuffer_Skip(svLinkBuffer_t *lb);

#endif // __SVLINKBUFFER_H__
