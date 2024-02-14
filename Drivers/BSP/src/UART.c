/*
 * UART.c
 *
 *  Created on: Jan 29, 2024
 *      Author: mm940
 */


#include "UART.h"
#include "BSPConfig.h"
#include "LED.h"
#include <stdarg.h>
#include <stdio.h>
#include "svRingBuffer.h"
#include "svDebug.h"



static uint8_t rx_data;

// change to array - UART ch
static BOOL s_bUARTConnected = FALSE;

UART_HandleTypeDef huart1;

void  UART_Init(uint8_t ch, uint32_t baud)
{
  switch(ch)
  {
    case _DEF_UART1:
      huart1.Instance = USART1;
      huart1.Init.BaudRate = baud;
      huart1.Init.WordLength = UART_WORDLENGTH_8B;
      huart1.Init.StopBits = UART_STOPBITS_1;
      huart1.Init.Parity = UART_PARITY_NONE;
      huart1.Init.Mode = UART_MODE_TX_RX;
      huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
      huart1.Init.OverSampling = UART_OVERSAMPLING_16;
      huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
      huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
      huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
      if (HAL_UART_Init(&huart1) != HAL_OK)
      {
        Error_Handler();
        s_bUARTConnected = FALSE;
        g_bDebugTx = FALSE;
      }
      else if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
      {
        Error_Handler();
        s_bUARTConnected = FALSE;
        g_bDebugTx = FALSE;
      }
      else if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
      {
        Error_Handler();
        s_bUARTConnected = FALSE;
        g_bDebugTx = FALSE;
      }
      else if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
      {
        Error_Handler();
        s_bUARTConnected = FALSE;
        g_bDebugTx = FALSE;
      }
      else
      {
        // ring buffer
        svRingBuffer_Reset(&g_uartRxBuff);
      }

      if(HAL_UART_Receive_IT(&huart1, (uint8_t*)&rx_data,1) != HAL_OK)
      {
        s_bUARTConnected = FALSE;
        g_bDebugTx = FALSE;
      }
      else
      {
        s_bUARTConnected = TRUE;
        g_bDebugTx = TRUE;
      }
      break;
    default:
      break;
  }
  return;
}

BOOL  IsUartConnected(uint8_t ch)
{
  return s_bUARTConnected;
}

uint32_t  UART_Write(uint8_t ch, uint8_t *Buf, uint16_t Len)
{
  uint32_t ret = 0;

  HAL_StatusTypeDef status;

  switch(ch)
  {
    case _DEF_UART1:
      status = HAL_UART_Transmit(&huart1, Buf, Len, 100);

      if(status == HAL_OK)
      {
        ret = Len;
        g_bDebugTx = TRUE;
      }
      break;
    default:
      break;
  }
  return ret;
}


uint32_t  UART_GetBaud(void)
{
  uint32_t ret = 0;
  ret = huart1.Init.BaudRate;

  return ret;
}


uint32_t UART_Printf(uint8_t ch, char* fmt, ...) // ... arg
{
  char buf[256];
  va_list args;

  int len;
  uint32_t ret;

  va_start(args, fmt);
  // printf 결과를 buf 에 저장
  len = vsnprintf(buf, 256, fmt, args);

  ret = UART_Write(ch, (uint8_t *)buf, len);

  va_end(args);

  return ret;
}

uint32_t Uart_Available(uint8_t ch)
{
  uint32_t ret = 0;

  switch(ch)
  {
    case _DEF_UART1:
      ret = svRingBuffer_GetCount(&g_uartRxBuff);
      break;
  }

  return ret;
}
uint8_t Uart_Read(uint8_t ch)
{
  uint8_t ret = 0;

  switch(ch)
  {
    case _DEF_UART1:
      ret = svRingBuffer_Pop(&g_uartRxBuff);
      break;
  }

  return ret;
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == USART1)
  {
    // For UART Test
    // UART_Printf(_DEF_UART1, "RxData : 0x%X\n", rx_data);
    int result = svRingBuffer_Push(&g_uartRxBuff, rx_data);
    if(result == -2 )
    {
      svRingBuffer_Reset(&g_uartRxBuff);
    }else
    {

    }
    HAL_UART_Receive_IT(&huart1, (uint8_t *)&rx_data, 1);
  }
}
