/*
 * RTC.c
 *
 *  Created on: Jan 29, 2024
 *      Author: mm940
 */

#include "RTC.h"

static RTC_HandleTypeDef hrtc;

BOOL RTC_Init()
{
  BOOL ret = TRUE;

  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  hrtc.Init.OutPutPullUp = RTC_OUTPUT_PULLUP_NONE;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  return ret;
}

uint32_t RTC_BackupRegRead(uint32_t index)
{
  return HAL_RTCEx_BKUPRead(&hrtc, index);
}
void RTC_BackupRegWrite(uint32_t index, uint32_t data)
{
  HAL_RTCEx_BKUPWrite(&hrtc, index, data);
}
