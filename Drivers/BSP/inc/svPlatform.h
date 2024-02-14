////////////////////////////////////////////////////////////////////////////////
// Author : Solvine
// Creation Date : 220522
// File : svPlatform.h
// Desc. : Platform specific primitive driver
//
////////////////////////////////////////////////////////////////////////////////

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SVPLATFORM_H__
#define __SVPLATFORM_H__

#include "svUtils.h"

// Definitions
#define TIMER_ID_LED_RX_ON					SV_TIMER_1
#define TIMER_ID_LED_RX_OFF					SV_TIMER_2
#define TIMER_ID_CAN_RX0_ON         SV_TIMER_3
#define TIMER_ID_CAN_RX0_OFF        SV_TIMER_4
#define TIMER_ID_CAN_RX1_ON         SV_TIMER_5
#define TIMER_ID_CAN_RX1_OFF        SV_TIMER_6
#define TIMER_INTERVAL_LED_RX_ON		100
#define TIMER_INTERVAL_LED_RX_OFF		100
#define TIMER_INTERVAL_CAN_RX0_ON   100
#define TIMER_INTERVAL_CAN_RX0_OFF  100
#define TIMER_INTERVAL_CAN_RX1_ON   50
#define TIMER_INTERVAL_CAN_RX1_OFF  50
// helper functions
extern uint32_t GetElapsedTime(uint32_t tickRef);

#endif	// __SVPLATFORM_H__
