/*
 * LED.c
 *
 *  Created on: Jan 29, 2024
 *      Author: mm940
 */

#include "BSPConfig.h"
#include "main.h"
#include "LED.h"
#include "svCLI.h"
#include "svDebug.h"

// Static Variables
static BOOL s_bLEDInit = FALSE;
static BOOL s_bLEDEnable = FALSE;

// Function Declarations
void LED_Init(void)
{
  s_bLEDInit = TRUE;
  s_bLEDEnable = TRUE;
}

void LED_Deinit(void)
{
  s_bLEDInit = FALSE;
}

void LED_Enable(BOOL bEnable)
{
  s_bLEDEnable = bEnable;
}

void LED_OnOff(LED_e eLED, BOOL bOn)
{
  if(FALSE == s_bLEDEnable)
  {
    return;
  }

  GPIO_TypeDef* LED_GPIO_Port;
  uint16_t LED_GPIO_Pin;
  if(s_bLEDInit)
  {
    switch(eLED){
      case LED_TX:
        LED_GPIO_Port = PA5_LD2_LED_GPIO_Port;
        LED_GPIO_Pin = PA5_LD2_LED_Pin;
        break;
      default:
        return;
        break;
    }
    if(bOn)
    {
      HAL_GPIO_WritePin(LED_GPIO_Port, LED_GPIO_Pin, GPIO_PIN_SET);
    }
    else
    {
      HAL_GPIO_WritePin(LED_GPIO_Port, LED_GPIO_Pin, GPIO_PIN_RESET);
    }
  }
}

void LED_AllOnOff(BOOL bOn)
{
  if(FALSE == s_bLEDEnable)
  {
    return;
  }
  LED_OnOff(LED_TX, bOn);
}

void LED_SimpleAnimation(void)
{
  for(int i =0; i<3; i++)
  {
    LED_OnOff(LED_TX, TRUE);
    HAL_Delay(50);
    LED_OnOff(LED_TX, FALSE);
    HAL_Delay(50);
  }
}

#ifdef USE_CLI_FUNCTIONS
SVCLI_TABLE_ENTRY (LED_TEST,"LED","LED [1~11] [0,1,2] ",CLI_LEDTest);

static void CLI_LEDTest(char* pArgLine, unsigned int nCount, unsigned int nFirst, unsigned int nSecond)
{
  if (nCount == 2) {
    LED_OnOff((LED_e)nFirst, (BOOL)(nSecond != 0));
  }
}
#endif // USE_CLI_FUNCTIONS
