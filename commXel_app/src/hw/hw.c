/*
 * hw.c
 *
 *  Created on: 2017. 2. 13.
 *      Author: baram
 */



#include "hw.h"





//-- Internal Variables
//


//-- External Variables
//


//-- Internal Functions
//


//-- External Functions
//


void hwInit(void)
{
  cmdifInit();
  qbufferInit();

  swtimerInit();
  timerInit();

  ledInit();
  buttonInit();

  bspInitUSB();

  uartInit();
//  eepromInit();
//  flashInit();
  resetInit();
  dxlportInit();
  dxlcmdInit();
}

void delay(uint32_t delay_ms)
{
#ifdef _USE_HW_RTOS
  if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
  {
    osDelay(delay_ms);
  }
  else
  {
    HAL_Delay(delay_ms);
  }
#else
  HAL_Delay(delay_ms);
#endif
}

void delayMillis(uint32_t delay_ms)
{
  HAL_Delay(delay_ms);
}

void delayMicros(uint32_t delay_us)
{
  uint32_t tickstart = 0;


  tickstart = micros();
  while((micros() - tickstart) < delay_us)
  {
  }
}

void delaySeconds(uint32_t delay_sec)
{

}

uint32_t millis(void)
{
  return HAL_GetTick();
}

uint32_t micros(void)
{
  return timerGetMicros();
}

