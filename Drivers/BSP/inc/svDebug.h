////////////////////////////////////////////////////////////////////////////////
// Author : Solvine
// Creation Date : 220521
// File : svDebug.h
// Desc. : Debug message related stuffs
//
////////////////////////////////////////////////////////////////////////////////

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "BSPConfig.h"
#include "ASCII_escape.h"
#include "svRingBuffer.h"
#include "svCLI.h"

#ifdef BOOTLOADER_NO_DEBUG___
#define DEBUG_ENABLED			0
#else
#define DEBUG_ENABLED			1
#endif

// Definitions
#define DEBUG_LEVEL_0		1
#define DEBUG_LEVEL_1		2
#define DEBUG_LEVEL_2		3
#define DEBUG_LEVEL_3		4
//#define DEBUG_LEVEL_4		5
//#define DEBUG_LEVEL_5		6

#define DEBUG_VERBOSE							DEBUG_LEVEL_3
#define DEBUG_MESSAGE_NOTHING			DEBUG_LEVEL_0

#define DEBUGMSG_VERBOSE					DEBUG_LEVEL_3
#define DEBUGMSG_NORMAL						DEBUG_LEVEL_2
#define DEBUGMSG_APP							DEBUG_LEVEL_1
#define DEBUGMSG_CLI							DEBUG_LEVEL_0
#define DEBUGMSG_CRITICAL					DEBUG_LEVEL_0
#define DEBUGMSG_ERROR						DEBUG_LEVEL_0
#define DEBUGMSG_SKIP							(DEBUGMSG_VERBOSE + 1)

#define svDebugPrintf_LFCR()		svDebugPutString("\r\n")
// Macros
extern uint32_t g_DebugMsgLevel;
extern svRingBuffer_t	g_rbDebugRx;
extern BOOL g_bDebugTx;

#if DEBUG_ENABLED == 1
#define DebugMsg(level,...) do { if (level <= g_DebugMsgLevel) \
															svDebugMessage(__VA_ARGS__);\
														} while(0)
#define ErrorMsg(...) 	do { svDebugPutString(AES_BRIGHT_RED_FG); \
													svDebugMessage(__VA_ARGS__); \
													svDebugPutString(AES_RESET);\
												} while(0)
#else
	#define DebugMsg(level,...)			do {} while(0)
	#define ErrorMsg(...)		do {} while(0)
#endif

// Function Declarations
void svDebugInit(void);
void svDebugProcess(void);
size_t svDebugGetTxFree(void);		// hellosmk 220523
char svDebugGetChar(void);
void svDebugPutChar(char cPut);
void svDebugPutString(char* szMsg);
void svDebugMessage(char* fmt, ...);
void svSetDebugMsgLevel(uint32_t uDebugMsgLevel);
void svDebugFormattedString(uint8_t* byData, int nDataLength, int nDispLength);

#endif	// __DEBUG_H__

