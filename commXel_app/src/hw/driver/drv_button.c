/*
 * drv_button.c
 *
 *  Created on: 2017. 2. 13.
 *      Author: baram
 */

#include "hw.h"
#include "drv_button.h"



typedef struct
{
  GPIO_TypeDef       *port;
  uint16_t            pin;
}drv_button_gpio_t;


typedef struct
{
  GPIO_PinState       on_state;
  drv_button_gpio_t   gpio;
}drv_button_t;



//-- Internal Variables
//
drv_button_t drv_button_tbl[BUTTON_MAX_CH];


//-- External Variables
//


//-- Internal Functions
//


//-- External Functions
//



bool drvButtonInit(void)
{
  uint32_t i;
  GPIO_InitTypeDef  GPIO_InitStruct;

  drv_button_tbl[0].gpio.port = GPIOC;
  drv_button_tbl[0].gpio.pin  = GPIO_PIN_12;
  drv_button_tbl[0].on_state  = GPIO_PIN_SET;

  GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

  for (i=0; i<BUTTON_MAX_CH; i++)
  {
    if (drv_button_tbl[i].gpio.port != NULL)
    {
      GPIO_InitStruct.Pin = drv_button_tbl[i].gpio.pin;
      HAL_GPIO_Init(drv_button_tbl[i].gpio.port, &GPIO_InitStruct);
    }
  }
  return true;
}

uint8_t drvButtonGetState(uint8_t ch)
{
  uint8_t ret = 0;

  if (HAL_GPIO_ReadPin(drv_button_tbl[ch].gpio.port, drv_button_tbl[ch].gpio.pin) == drv_button_tbl[ch].on_state)
  {
    ret = 1;
  }
  else
  {
    ret = 0;
  }

  return ret;
}


