/*
 * Flash.h
 *
 *  Created on: Jan 29, 2024
 *      Author: mm940
 */

#ifndef BSP_INC_FLASH_H_
#define BSP_INC_FLASH_H_

#include "BSPConfig.h"

BOOL Flash_Init(void);
BOOL Flash_Erase(uint32_t addr, uint32_t length);
BOOL Flash_Write(uint32_t addr, uint8_t *p_data, uint32_t length);
BOOL Flash_Read(uint32_t addr, uint32_t *p_data, uint32_t length);
#endif /* BSP_INC_FLASH_H_ */
