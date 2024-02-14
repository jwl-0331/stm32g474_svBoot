////////////////////////////////////////////////////////////////////////////////
// Author : Solvine
// Creation Date : 210210
// File : svUtils.h
// Desc. : Utility functions header
//
////////////////////////////////////////////////////////////////////////////////

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SVUTILS_H__
#define __SVUTILS_H__

#ifndef __BOOL_DEF__
#define __BOOL_DEF__
// BOOL_Reserved < Prevents enum down-size compiler optimization.
typedef enum { FALSE = 0, TRUE = 1 , BOOL_Reserved = 0x7FFFFFFFU} BOOL;
#endif	// __BOOL_DEF__

// System Timers must precede User Timers
typedef enum {
	SV_TIMER_1 = 0,
	SV_TIMER_2 = 1,
	SV_TIMER_3 = 2,
	SV_TIMER_4 = 3,
	SV_TIMER_5 = 4,
	SV_TIMER_6 = 5,
	SV_TIMER_7 = 6,
	SV_TIMER_8 = 7,
	SV_TIMER_9 = 8,
	SV_TIMER_10 = 9,
	SV_TIMER_11 = 10,
	SV_TIMER_12 = 11,
	SV_TIMER_13 = 12,
	SV_TIMER_14 = 13,
	SV_TIMER_15 = 14,
	SV_TIMER_16 = 15,
	SV_TIMER_MAX = 16					// 16 Timers
} svTimer_e;

typedef enum {
	SV_TIMER_ONCE 		= 0,
	SV_TIMER_REPEAT	= 1
} svTimerSet_e;

typedef void (*pfnUtilTimerCallback)(svTimer_e eTimerID);

#if defined ( __ICCARM__ )
typedef unsigned int (*SVUTIL_TIMETICK_FUNC_t)(void);

#elif defined ( __GNUC__ )
typedef long unsigned int (*SVUTIL_TIMETICK_FUNC_t)(void);

#endif

typedef struct {
	unsigned int 	dwDuration;		// Saved value for repeat ==> if 0 then No-Repeat
	unsigned int 	dwTimeOut;
	unsigned int	dwWrapAroundTick;	// Check flag which indicated Wrap-Around
	pfnUtilTimerCallback	pfnCallback;
	svTimer_e eTimerID;
	BOOL  	bLive;						// flag which indicates Running
} svUtilTimer_t;

extern void * utf8_decode(void *buf, unsigned int *c, int *e);

// Timer Related helper functions
extern void svUtil_InitTimers(SVUTIL_TIMETICK_FUNC_t pfnTimeTick);
extern void svUtil_KillAllTimers(void);
extern BOOL svUtil_SetTimer(svTimer_e eTimerID, unsigned int dwDuration, svTimerSet_e eTimerSet, pfnUtilTimerCallback pfnCallback);
extern BOOL svUtil_KillTimer(svTimer_e eTimerID);
extern BOOL svUtil_IsTimerRunning(svTimer_e eTimerID);
extern BOOL svUtil_CheckTimers(void);
extern svUtilTimer_t* svUtil_GetTimerInfo(svTimer_e eTimerID);	// hellosmk 220112
#endif	// __SVUTILS_H__
