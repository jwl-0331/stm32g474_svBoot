/*
 * UART.h
 *
 *  Created on: Jan 29, 2024
 *      Author: mm940
 */

#ifndef BSP_INC_UART_H_
#define BSP_INC_UART_H_

#include "BSPConfig.h"

#define _DEF_UART1 0
#define g_uartRxBuff g_rbDebugRx

void      UART_Init(uint8_t ch, uint32_t baud);
BOOL      IsUartConnected(uint8_t ch);
uint32_t  UART_Write(uint8_t ch, uint8_t *Buf, uint16_t Len);
uint32_t  UART_Printf(uint8_t ch, char* fmt, ...);
uint32_t  UART_GetBaud(void);

#endif /* BSP_INC_UART_H_ */
