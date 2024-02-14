/*
 * Reset.h
 *
 *  Created on: Jan 29, 2024
 *      Author: mm940
 */

#ifndef BSP_INC_RESET_H_
#define BSP_INC_RESET_H_

#include "BSPConfig.h"

#define RESET_REG_PARAM   0
#define RESET_REG_COUNT   1

BOOL Reset_Init(void);

uint32_t Reset_GetCount(void);
void Reset_ToBoot(uint32_t timeout);
void Reset_ToSysBoot(void);

#endif /* BSP_INC_RESET_H_ */
