/*
 * dxlport.c
 *
 *  Created on: 2017. 2. 13.
 *      Author: baram
 */



#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

#include "hw.h"
#include "drv_dxlport.h"

#ifndef _USE_HW_UART
#error "You must use UART for this function"
#endif

#include "uart.h"
#include "drv_uart.h"



typedef struct
{
  uint8_t ch;
} dxlport_t;


volatile static bool tx_done[DXLPORT_MAX_CH];


//-- Internal Variables
//
dxlport_t dxlport_tbl[DXLPORT_MAX_CH] = {
  {_DEF_UART2},   // DXL1 -> DXL Port
  {_DEF_UART1},   // DXL2 -> DXL Port
};

static void  (*txDoneISR[DXLPORT_MAX_CH])(void);


//-- External Variables
//


//-- Internal Functions
//



//-- External Functions
//




bool drvDxlportInit(void)
{
  uint8_t i;
  GPIO_InitTypeDef  GPIO_InitStruct;


  // DXL_TX_ENABLE
  //
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

  GPIO_InitStruct.Pin = GPIO_PIN_8;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);


  for (i=0; i<DXLPORT_MAX_CH; i++)
  {
    drvDxlportTxDisable(i);

    txDoneISR[i] = NULL;
    tx_done[i] = false;
  }

  return true;
}

bool drvDxlportOpen(uint8_t ch, uint32_t baud)
{
  bool ret = true;

  if (ch >= DXLPORT_MAX_CH) return false;


  uartOpen(dxlport_tbl[ch].ch, baud);

  //if (ch == _DEF_DXL1)
  //{
  //  drvUartSetTxDoneISR(dxlport_tbl[ch].ch, drvDxlportTxDoneISR1);
  //}

  return ret;
}

uint32_t drvDxlportGetBaud(uint8_t ch)
{
  return drvUartGetBaud(dxlport_tbl[ch].ch);
}

bool drvDxlportClose(uint8_t ch)
{
  bool ret = true;

  if (ch >= DXLPORT_MAX_CH) return false;


  uartClose(dxlport_tbl[ch].ch);

  return ret;
}

void drvDxlportSetTxDoneISR(uint8_t ch, void (*p_txDoneISR)(void))
{
  if (ch >= DXLPORT_MAX_CH) return;

  txDoneISR[ch] = p_txDoneISR;
}

bool drvDxlportFlush(uint8_t ch)
{
  bool ret = true;

  if (ch >= DXLPORT_MAX_CH) return false;


  uartFlush(dxlport_tbl[ch].ch);

  return ret;
}

void drvDxlportTxEnable(uint8_t ch)
{

  switch(ch)
  {
    case _DEF_DXL1:
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
      break;

    default:
      break;
  }
}

void drvDxlportTxDisable(uint8_t ch)
{

  switch(ch)
  {
    case _DEF_DXL1:
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
      break;

    default:
      break;
  }
}

uint32_t drvDxlportAvailable(uint8_t ch)
{
  uint32_t ret = 0;


  if (ch >= DXLPORT_MAX_CH) return 0;


  ret = uartAvailable(dxlport_tbl[ch].ch);

  return ret;
}

uint8_t drvDxlportRead(uint8_t ch)
{
  uint8_t ret = 0;


  if (ch >= DXLPORT_MAX_CH) return 0;


  ret = uartRead(dxlport_tbl[ch].ch);

  return ret;
}

bool drvDxlportIsTxDone(uint8_t ch)
{
  return tx_done[ch];
}

uint32_t drvDxlportWrite(uint8_t ch, uint8_t *p_data, uint32_t length)
{
  uint32_t ret = 0;


  if (ch >= DXLPORT_MAX_CH) return 0;

  tx_done[ch] = false;

  drvDxlportTxEnable(ch);


  ret = uartWrite(dxlport_tbl[ch].ch, p_data, length);


  // DXL1은 데이터 송신을 DMA를 사용함으로써 TxDisable은 데이터 종료 인터럽트에서 실행.
  //
  //if (ch != _DEF_DXL1)
  {
    drvDxlportTxDisable(ch);
    tx_done[ch] = true;
  }

  return ret;
}
