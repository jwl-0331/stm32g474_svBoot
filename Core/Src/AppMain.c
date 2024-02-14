/*
 * AppMain.c
 *
 *  Created on: Jan 29, 2024
 *      Author: mm940
 */
#include "main.h"
#include "BSPConfig.h"

#include "LED.h"
#include "UART.h"
#include "Reset.h"
#include "RTC.h"
#include "Flash.h"

void AppMain(void)
{
  Reset_Init();
  UART_Init(_DEF_UART1, 115200);
  uint32_t pre_time = HAL_GetTick();
  while(1)
  {
    if(HAL_GetTick() - pre_time >= 300)
    {
      pre_time = HAL_GetTick();
      HAL_GPIO_TogglePin(PA5_LD2_LED_GPIO_Port, PA5_LD2_LED_Pin);
    }
    if(Reset_GetCount() == 2)
    {
      Reset_ToBoot(500);
    }

  }
}

