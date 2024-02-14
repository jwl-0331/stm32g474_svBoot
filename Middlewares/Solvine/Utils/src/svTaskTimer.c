////////////////////////////////////////////////////////////////////////////////
// Author : Solvine
// Creation Date : 210606
// File : svTaskTimer.c
// Desc. : Task Based Timer functions
//
////////////////////////////////////////////////////////////////////////////////

#include "svTaskTimer.h"

// Global variables
// Static Variables
// Functions
void svTaskTimer_InitTimers(svTaskTimer_t* pTaskTimer, SVUTIL_TIMETICK_FUNC_t pfnTimeTick)
{
	// Note. Argument Integrity Not Check !!!
	pTaskTimer->pfnTimeTick = pfnTimeTick;
	for (int i=SV_TIMER_1;i<pTaskTimer->uNumOfTimers;i++) {
		pTaskTimer->psvUtilTimer[i].dwDuration = 0;
		pTaskTimer->psvUtilTimer[i].dwTimeOut = 0;
		pTaskTimer->psvUtilTimer[i].bLive = FALSE;
		pTaskTimer->psvUtilTimer[i].dwWrapAroundTick = 0;
		pTaskTimer->psvUtilTimer[i].eTimerID = (svTimer_e)i;
		pTaskTimer->psvUtilTimer[i].pfnCallback = 0;
	}
}

void svTaskTimer_KillAllTimers(svTaskTimer_t* pTaskTimer)
{
	for (int i=SV_TIMER_1;i<pTaskTimer->uNumOfTimers;i++) {
		pTaskTimer->psvUtilTimer[i].bLive = FALSE;
	}
}

BOOL svTaskTimer_SetTimer(svTaskTimer_t* pTaskTimer, svTimer_e eTimerID, unsigned int dwDuration, svTimerSet_e eTimerSet, pfnUtilTimerCallback pfnCallback)
{
	if (dwDuration == 0) {
		return svTaskTimer_KillTimer(pTaskTimer, eTimerID);
	}
	if (eTimerID >= pTaskTimer->uNumOfTimers) {
		return FALSE;
	}
	// Check Wrap around !!!
	unsigned int dwCurrentTick = pTaskTimer->pfnTimeTick();
	pTaskTimer->psvUtilTimer[eTimerID].dwTimeOut = dwCurrentTick + dwDuration;

	if (pTaskTimer->psvUtilTimer[eTimerID].dwTimeOut < dwCurrentTick) {	// Wrap around
			pTaskTimer->psvUtilTimer[eTimerID].dwWrapAroundTick = dwCurrentTick;
	}
	else {
			pTaskTimer->psvUtilTimer[eTimerID].dwWrapAroundTick = 0;
	}
	if (eTimerSet == SV_TIMER_REPEAT) {
		pTaskTimer->psvUtilTimer[eTimerID].dwDuration = dwDuration;
	}
	else {
		pTaskTimer->psvUtilTimer[eTimerID].dwDuration = 0;
	}
	pTaskTimer->psvUtilTimer[eTimerID].pfnCallback = pfnCallback;
	pTaskTimer->psvUtilTimer[eTimerID].bLive = TRUE;		// Set Running
	return TRUE;
}

BOOL svTaskTimer_KillTimer(svTaskTimer_t* pTaskTimer, svTimer_e eTimerID)
{
	if (eTimerID >= pTaskTimer->uNumOfTimers) {
		return FALSE;
	}
	pTaskTimer->psvUtilTimer[eTimerID].bLive = FALSE;		// Set Non-Running
	return TRUE;
}

BOOL svTaskTimer_IsTimerRunning(svTaskTimer_t* pTaskTimer, svTimer_e eTimerID)
{
	if (eTimerID < pTaskTimer->uNumOfTimers) {
		return (pTaskTimer->psvUtilTimer[eTimerID].bLive);
	}
	return FALSE;
}

BOOL svTaskTimer_CheckTimers(svTaskTimer_t* pTaskTimer)
{
	unsigned int dwCurrentTick = pTaskTimer->pfnTimeTick();
	for (int i=SV_TIMER_1;i<pTaskTimer->uNumOfTimers;i++) {
		// Check Running Timer
		if (pTaskTimer->psvUtilTimer[i].bLive) {
			if (pTaskTimer->psvUtilTimer[i].dwWrapAroundTick > 0) {	// Wrap around
				if (dwCurrentTick >= pTaskTimer->psvUtilTimer[i].dwWrapAroundTick)
					continue;
			}
			if (pTaskTimer->psvUtilTimer[i].dwTimeOut <= dwCurrentTick) {	// Expired !!!
				if (pTaskTimer->psvUtilTimer[i].dwDuration > 0) {	// Repeat timer
					svTaskTimer_SetTimer(pTaskTimer, (svTimer_e)i, pTaskTimer->psvUtilTimer[i].dwDuration, SV_TIMER_REPEAT, pTaskTimer->psvUtilTimer[i].pfnCallback); 				
				}
				else {	// Stop Timer
					pTaskTimer->psvUtilTimer[i].bLive = FALSE;
				}
				// Call Callback function !!
				if (pTaskTimer->psvUtilTimer[i].pfnCallback) {
					pTaskTimer->psvUtilTimer[i].pfnCallback((svTimer_e)i);
				}
			}
		}
	}
	return TRUE;
}


