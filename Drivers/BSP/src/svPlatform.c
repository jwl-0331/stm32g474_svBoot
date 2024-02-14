////////////////////////////////////////////////////////////////////////////////
// Author : Solvine
// Creation Date : 220522
// File : Platform.c
// Desc. : Platform specific primitive driver
//
////////////////////////////////////////////////////////////////////////////////
#include "BSPConfig.h"
#include "svPlatform.h"
#include "main.h"
#include "svDebug.h"
#include "LED.h"
#include "svLinkBuffer.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


// Functions
uint32_t GetElapsedTime(uint32_t tickRef)
{
	uint32_t tickCurrent = HAL_GetTick();
	// Check Wrap around
	if (tickCurrent < tickRef) {
		return (0xffffffff - tickRef + tickCurrent);
	}
	else {
		return tickCurrent - tickRef;
	}
}

