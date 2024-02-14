////////////////////////////////////////////////////////////////////////////////
// Author : Solvine
// Creation Date : 210210
// File : svUtils.C
// Desc. : Utility functions
//
////////////////////////////////////////////////////////////////////////////////

#include "svUtils.h"

// Global variables
// Static Variables
static svUtilTimer_t		s_svUtilTimer[SV_TIMER_MAX];  //17
SVUTIL_TIMETICK_FUNC_t	s_pfnGetTick;
// Functions
void * utf8_decode(void *buf, unsigned int *c, int *e)
{
	static const char lengths[] = {
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 3, 3, 4, 0
	};
	static const int masks[] = { 0x00, 0x7f, 0x1f, 0x0f, 0x07 };
	static const unsigned int mins[] = { 4194304, 0, 128, 2048, 65536 };
	static const int shiftc[] = { 0, 18, 12, 6, 0 };
	static const int shifte[] = { 0, 6, 4, 2, 0 };

	unsigned char *s = (unsigned char *)buf;
	int len = lengths[s[0] >> 3];

	/* Compute the pointer to the next character early so that the next
	 * iteration can start working on the next character. Neither Clang
	 * nor GCC figure out this reordering on their own.
	 */
	unsigned char *next = s + len + !len;

	/* Assume a four-byte character and load four bytes. Unused bits are
	 * shifted out.
	 */
	*c = (unsigned int)(s[0] & masks[len]) << 18;
	*c |= (unsigned int)(s[1] & 0x3f) << 12;
	*c |= (unsigned int)(s[2] & 0x3f) << 6;
	*c |= (unsigned int)(s[3] & 0x3f) << 0;
	*c >>= shiftc[len];

	/* Accumulate the various error conditions. */
	*e = (*c < mins[len]) << 6; // non-canonical encoding
	*e |= ((*c >> 11) == 0x1b) << 7;  // surrogate half?
	*e |= (*c > 0x10FFFF) << 8;  // out of range?
	*e |= (s[1] & 0xc0) >> 2;
	*e |= (s[2] & 0xc0) >> 4;
	*e |= (s[3]) >> 6;
	*e ^= 0x2a; // top two bits of each tail byte correct?
	*e >>= shifte[len];

	return next;
}

void svUtil_InitTimers(SVUTIL_TIMETICK_FUNC_t pfnTimeTick)
{
	s_pfnGetTick = pfnTimeTick;
	for (int i=SV_TIMER_1;i<SV_TIMER_MAX;i++) {
		s_svUtilTimer[i].dwDuration = 0;          // 0 - No repeat, Repeat Value
		s_svUtilTimer[i].dwTimeOut = 0;
		s_svUtilTimer[i].bLive = FALSE;           // running
		s_svUtilTimer[i].dwWrapAroundTick = 0;    // Wrap-Around (timer_init)
		s_svUtilTimer[i].eTimerID = (svTimer_e)i; // TimerID - 0 ~ 16
		s_svUtilTimer[i].pfnCallback = 0;         // Timer 만료 실행 되는 함수?
	}
}

void svUtil_KillAllTimers(void){
	for (int i=SV_TIMER_1;i<SV_TIMER_MAX;i++) {
		s_svUtilTimer[i].bLive = FALSE;
	}
}

/* here */
BOOL svUtil_SetTimer(svTimer_e eTimerID, unsigned int dwDuration, svTimerSet_e eTimerSet, pfnUtilTimerCallback pfnCallback)
{
	if (dwDuration == 0)
		return svUtil_KillTimer(eTimerID);
	if (eTimerID >= SV_TIMER_MAX)
		return FALSE;
	// Check Wrap around !!!
	unsigned int dwCurrentTick = s_pfnGetTick();
	s_svUtilTimer[eTimerID].dwTimeOut = dwCurrentTick + dwDuration;

	if (s_svUtilTimer[eTimerID].dwTimeOut < dwCurrentTick) {	// Wrap around
			s_svUtilTimer[eTimerID].dwWrapAroundTick = dwCurrentTick;
	}
	else
			s_svUtilTimer[eTimerID].dwWrapAroundTick = 0;
	
	if (eTimerSet == SV_TIMER_REPEAT)
		s_svUtilTimer[eTimerID].dwDuration = dwDuration;
	else
		s_svUtilTimer[eTimerID].dwDuration = 0;

	s_svUtilTimer[eTimerID].pfnCallback = pfnCallback;
	s_svUtilTimer[eTimerID].bLive = TRUE;		// Set Running
	return TRUE;
}

BOOL svUtil_KillTimer(svTimer_e eTimerID)
{
	if (eTimerID >= SV_TIMER_MAX)
		return FALSE;
	s_svUtilTimer[eTimerID].bLive = FALSE;		// Set Non-Running
	return TRUE;
}

BOOL svUtil_IsTimerRunning(svTimer_e eTimerID)
{
	if (eTimerID < SV_TIMER_MAX) {
		return (s_svUtilTimer[eTimerID].bLive);
	}
	return FALSE;
}

BOOL svUtil_CheckTimers(void)
{
	unsigned int dwCurrentTick = s_pfnGetTick();
	for (int i=SV_TIMER_1;i<SV_TIMER_MAX;i++) {
		// Check Running Timer
		if (s_svUtilTimer[i].bLive) {
			if (s_svUtilTimer[i].dwWrapAroundTick > 0) {	// Wrap around
				if (dwCurrentTick >= s_svUtilTimer[i].dwWrapAroundTick)
					continue;
			}
			if (s_svUtilTimer[i].dwTimeOut <= dwCurrentTick) {	// Expired !!!
				if (s_svUtilTimer[i].dwDuration > 0) {	// Repeat timer
					svUtil_SetTimer((svTimer_e)i, s_svUtilTimer[i].dwDuration, SV_TIMER_REPEAT, s_svUtilTimer[i].pfnCallback); 				
				}
				else {	// Stop Timer
					s_svUtilTimer[i].bLive = FALSE;
				}
				// Call Callback function !!
				if (s_svUtilTimer[i].pfnCallback) {
					s_svUtilTimer[i].pfnCallback((svTimer_e)i);
				}
			}
		}
	}
	return TRUE;
}

// hellosmk 220112
svUtilTimer_t* svUtil_GetTimerInfo(svTimer_e eTimerID)
{
	if (eTimerID < SV_TIMER_MAX) {
		return &s_svUtilTimer[eTimerID];
	}
	else {
		return 0;
	}
}

