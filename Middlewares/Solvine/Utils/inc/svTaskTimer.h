////////////////////////////////////////////////////////////////////////////////
// Author : Solvine
// Creation Date : 210606
// File : svTaskTimer.h
// Desc. : Task Based Timer functions header
//
////////////////////////////////////////////////////////////////////////////////

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SVTASKTIMER_H__
#define __SVTASKTIMER_H__

#include "svUtils.h"		// To Reuse Timer Related definitions

typedef struct {
	unsigned int	 					uNumOfTimers;
	SVUTIL_TIMETICK_FUNC_t	pfnTimeTick;
	svUtilTimer_t* 					psvUtilTimer;
} svTaskTimer_t;

#define svTaskTimer_DEF(_name,_size)         \
    svUtilTimer_t _name##_timer[_size];     \
    svTaskTimer_t _name = {              \
			.uNumOfTimers = _size,		\
			.psvUtilTimer = &_name##_timer[0],	\
		}

// Timer Related helper functions
extern void svTaskTimer_InitTimers(svTaskTimer_t* pTaskTimer, SVUTIL_TIMETICK_FUNC_t pfnTimeTick);
extern void svTaskTimer_KillAllTimers(svTaskTimer_t* pTaskTimer);
extern BOOL svTaskTimer_SetTimer(svTaskTimer_t* pTaskTimer, svTimer_e eTimerID, unsigned int dwDuration, svTimerSet_e eTimerSet, pfnUtilTimerCallback pfnCallback);
extern BOOL svTaskTimer_KillTimer(svTaskTimer_t* pTaskTimer, svTimer_e eTimerID);
extern BOOL svTaskTimer_IsTimerRunning(svTaskTimer_t* pTaskTimer, svTimer_e eTimerID);
extern BOOL svTaskTimer_CheckTimers(svTaskTimer_t* pTaskTimer);

#endif	// __SVTASKTIMER_H__

