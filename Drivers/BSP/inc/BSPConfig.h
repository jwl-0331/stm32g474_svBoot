/*
 * BSPConfig.h
 *
 *  Created on: Jan 29, 2024
 *      Author: mm940
 */

#ifndef BSP_INC_BSPCONFIG_H_
#define BSP_INC_BSPCONFIG_H_

#include "stm32g4xx_hal.h"

#ifndef __BOOL_DEF__
#define __BOOL_DEF__
typedef enum { FALSE = 0, TRUE = 1, BOOL_Reserverd = 0x7FFFFFFFU} BOOL;
#endif

#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#endif /* BSP_INC_BSPCONFIG_H_ */
