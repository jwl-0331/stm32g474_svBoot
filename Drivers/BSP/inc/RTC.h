/*
 * RTC.h
 *
 *  Created on: Jan 29, 2024
 *      Author: mm940
 */

#ifndef BSP_INC_RTC_H_
#define BSP_INC_RTC_H_

#include "BSPConfig.h"

BOOL RTC_Init();

uint32_t RTC_BackupRegRead(uint32_t index);
void RTC_BackupRegWrite(uint32_t intdex, uint32_t data);

#endif /* BSP_INC_RTC_H_ */
