/*
 * drv_uart.c
 *
 *  Created on: 2017. 2. 13.
 *      Author: baram
 */



#include <stdarg.h>
#include <stdbool.h>

#include "hw.h"
#include "drv_uart.h"

/*
  DRV_UART_CH_MAX : 2
  _DEF_UART1          : VCP
  _DEF_UART2          : USART1
*/
/*
  USART1
    - RX : PB7, DMA2, Stream5, Channel 4
    - TX : PB6, DMA2, Stream7, Channel 4
*/



#define DRV_UART_RX_BUF_LENGTH    1024




//-- Internal Variables
//
drv_uart_t  drv_uart_tbl[DRV_UART_CH_MAX];

uint8_t  drv_uart_rx_buf[DRV_UART_CH_MAX][DRV_UART_RX_BUF_LENGTH] __attribute__((section(".NoneCacheableMem")));


//-- External Variables
//


//-- Internal Functions
//
void drvUartStartRx(uint8_t channel);


//-- External Functions
//




//--------------------------------------- Driver Functions
//

bool drvUartInit(void)
{
  uint32_t i;


  for(i=0; i<DRV_UART_CH_MAX; i++)
  {
    drv_uart_tbl[i].core.init     = false;
    drv_uart_tbl[i].core.open     = false;
    drv_uart_tbl[i].hw.dma_enable = false;
    drv_uart_tbl[i].hw.vcp_enable = false;

    drv_uart_tbl[i].hw.tx_dma_enable = false;
    drv_uart_tbl[i].hw.tx_done       = false;
    drv_uart_tbl[i].hw.txDoneISR     = NULL;
  }

  return true;
}

bool drvUartIsEnable(uint8_t channel)
{
  return drv_uart_tbl[channel].core.init;
}

bool drvUartFlush(uint8_t channel)
{
  drv_uart_t *p_drv_uart = &drv_uart_tbl[channel];


  if (p_drv_uart->hw.vcp_enable == true)
  {
    vcpFlush();
  }
  else
  {
    if (p_drv_uart->hw.dma_enable == true)
    {
      p_drv_uart->hw.uart_buffer.ptr_in  = p_drv_uart->hw.uart_buffer.length - p_drv_uart->hw.hdma_rx.Instance->NDTR;
      p_drv_uart->hw.uart_buffer.ptr_out = p_drv_uart->hw.uart_buffer.ptr_in;
      memset(p_drv_uart->hw.uart_buffer.p_buf, 0x00, p_drv_uart->hw.uart_buffer.length);
    }
    else
    {
      p_drv_uart->hw.uart_buffer.ptr_out = 0;
      p_drv_uart->hw.uart_buffer.ptr_in  = 0;
      memset(p_drv_uart->hw.uart_buffer.p_buf, 0x00, p_drv_uart->hw.uart_buffer.length);
    }
  }


  return true;
}

bool drvUartOpen(uint8_t channel, uint32_t baud, uint32_t option)
{
  drv_uart_t *p_drv_uart = &drv_uart_tbl[channel];


  switch(channel)
  {
    case _DEF_UART1:
      p_drv_uart->hw.dma_enable  = false;
      p_drv_uart->hw.vcp_enable  = true;

      p_drv_uart->core.init      = true;
      break;

    case _DEF_UART2:
      if (channel == _DEF_UART2)
      {
        p_drv_uart->hw.h_uart_inst = USART1;
        p_drv_uart->hw.tx_dma_enable = false;
      }

      p_drv_uart->hw.dma_enable  = true;
      p_drv_uart->hw.vcp_enable  = false;

      p_drv_uart->hw.h_uart.Instance        = p_drv_uart->hw.h_uart_inst;
      p_drv_uart->hw.h_uart.Init.BaudRate   = baud;
      p_drv_uart->hw.h_uart.Init.WordLength = UART_WORDLENGTH_8B;
      p_drv_uart->hw.h_uart.Init.StopBits   = UART_STOPBITS_1;
      p_drv_uart->hw.h_uart.Init.Parity     = UART_PARITY_NONE;
      p_drv_uart->hw.h_uart.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
      p_drv_uart->hw.h_uart.Init.Mode       = UART_MODE_TX_RX;

      HAL_UART_DeInit(&p_drv_uart->hw.h_uart);
      HAL_UART_Init(&p_drv_uart->hw.h_uart);

      if (p_drv_uart->core.init == false)
      {
        p_drv_uart->hw.uart_buffer.ptr_in  = 0;
        p_drv_uart->hw.uart_buffer.ptr_out = 0;
        p_drv_uart->hw.uart_buffer.length  = DRV_UART_RX_BUF_LENGTH;
        p_drv_uart->hw.uart_buffer.p_buf   = drv_uart_rx_buf[channel];

      }
      p_drv_uart->core.init = true;
      p_drv_uart->core.open = true;

      drvUartStartRx(channel);

      break;
  }

  return true;
}

bool drvUartClose(uint8_t channel)
{
  drv_uart_t *p_drv_uart = &drv_uart_tbl[channel];
  bool ret = true;


  if (p_drv_uart->hw.vcp_enable == true)
  {
    p_drv_uart->core.open = false;
  }
  else
  {
    if (p_drv_uart->core.init == true && p_drv_uart->core.open == true)
    {
      HAL_UART_DeInit(&p_drv_uart->hw.h_uart);
      p_drv_uart->core.open = false;
    }
  }


  return ret;
}

void drvUartSetTxDoneISR(uint8_t channel, void (*func)(void))
{
  drv_uart_tbl[channel].hw.txDoneISR = func;
}

uint32_t drvUartGetBaud(uint8_t channel)
{
  return drv_uart_tbl[channel].hw.h_uart.Init.BaudRate;
}

uint8_t drvUartGetIndex(UART_HandleTypeDef *UartHandle)
{
  uint32_t i;
  uint8_t  ret = 0;

  for(i=0; i<DRV_UART_CH_MAX; i++)
  {
    if (UartHandle->Instance == drv_uart_tbl[i].hw.h_uart_inst)
    {
      ret = i;
    }
  }

  return ret;
}

uint32_t drvUartIsAvailable(uint8_t channel)
{
  uint32_t ret = 0;
  drv_uart_t *p_drv_uart = &drv_uart_tbl[channel];


  if (p_drv_uart->hw.vcp_enable == true)
  {
    ret = vcpAvailable();
  }
  else
  {
    if (p_drv_uart->hw.dma_enable == true)
    {
      p_drv_uart->hw.uart_buffer.ptr_in = p_drv_uart->hw.uart_buffer.length - p_drv_uart->hw.hdma_rx.Instance->NDTR;
      ret = qbufferAvailable(&p_drv_uart->hw.uart_buffer);
      /*
      length = (   DRV_UART_RX_BUF_LENGTH
                 + drv_uart_rx_buf_head[uart_num]
                 - drv_uart_rx_buf_tail[uart_num] ) % DRV_UART_RX_BUF_LENGTH;
      */


    }
    else
    {
      ret = qbufferAvailable(&p_drv_uart->hw.uart_buffer);
    }
  }

  return ret;
}

uint32_t drvUartIsTxAvailable(uint8_t channel)
{
  uint32_t ret = 0;



  return ret;
}

int32_t drvUartWrite(uint8_t channel, uint8_t *p_data, uint32_t length)
{
  int32_t ret = 0;
  drv_uart_t *p_drv_uart = &drv_uart_tbl[channel];


  if (p_drv_uart->hw.vcp_enable == true)
  {
    ret = vcpWrite(p_data, length);
  }
  else
  {
    if (p_drv_uart->hw.tx_dma_enable == true)
    {
      if (HAL_UART_Transmit_DMA(&p_drv_uart->hw.h_uart, p_data, length) == HAL_OK)
      {
        ret = length;
      }
    }
    else
    {
      if (HAL_UART_Transmit(&p_drv_uart->hw.h_uart, (uint8_t*)p_data, length, 100) == HAL_OK)
      {
        ret = length;
      }
    }
  }

  if (ret == length)
  {
    drv_uart_tbl[channel].core.tx_cnt += length;
  }
  else
  {
    drv_uart_tbl[channel].core.tx_retry_cnt++;
  }

  return ret;
}

uint8_t  drvUartRead(uint8_t channel)
{
  uint8_t ret = 0;
  drv_uart_t *p_drv_uart = &drv_uart_tbl[channel];


  if (p_drv_uart->hw.vcp_enable == true)
  {
    ret = vcpRead();
  }
  else
  {
    qbufferRead(&p_drv_uart->hw.uart_buffer, &ret, 1);
  }
  return ret;
}

uint32_t drvUartGetTxErrCount(uint8_t channel)
{
  return drv_uart_tbl[channel].core.tx_err_cnt;
}

uint32_t drvUartGetTxCount(uint8_t channel)
{
  return drv_uart_tbl[channel].core.tx_cnt;
}

uint32_t drvUartGetTxRetryCount(uint8_t channel)
{
  return drv_uart_tbl[channel].core.tx_retry_cnt;
}




//--------------------------------------- Private Functions
//
void drvUartStartRx(uint8_t channel)
{
  drv_uart_t *p_drv_uart = &drv_uart_tbl[channel];


  if(p_drv_uart->hw.dma_enable == true)
  {
    HAL_UART_Receive_DMA(&p_drv_uart->hw.h_uart, (uint8_t *)p_drv_uart->hw.uart_buffer.p_buf, p_drv_uart->hw.uart_buffer.length );
  }
  else
  {
    //HAL_UART_Receive_IT(&p_drv_uart->hw.h_uart, (uint8_t *)p_drv_uart->hw.uart_rx_buf, 1);
  }

}

void drvUartErrHandler(uint8_t channel)
{
  //drv_uart_t *p_drv_uart = &drv_uart_tbl[channel];


  drvUartFlush(channel);
  drvUartStartRx(channel);
}

uint32_t  drvUartGetRxCount(uint8_t channel)
{
  return drv_uart_tbl[channel].core.rx_cnt;
}




//--------------------------------------- Hardware Functions
//


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  if (UartHandle->Instance == drv_uart_tbl[_DEF_UART2].hw.h_uart_inst)
  {
    drv_uart_tbl[_DEF_UART2].hw.tx_done = true;
    if (drv_uart_tbl[_DEF_UART2].hw.txDoneISR != NULL)
    {
      (*drv_uart_tbl[_DEF_UART2].hw.txDoneISR)();
    }
  }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
  //-- 에러발생시 복구함
  HAL_UART_DeInit(UartHandle);
  HAL_UART_Init(UartHandle);

  drvUartErrHandler(drvUartGetIndex(UartHandle));
}


// _DEF_UART2
//
void USART1_IRQHandler(void)
{
  HAL_UART_IRQHandler(&drv_uart_tbl[_DEF_UART2].hw.h_uart);
}

void DMA2_Stream5_IRQHandler(void)
{
  HAL_DMA_IRQHandler(drv_uart_tbl[_DEF_UART2].hw.h_uart.hdmarx);
}

void DMA2_Stream7_IRQHandler(void)
{
  HAL_DMA_IRQHandler(drv_uart_tbl[_DEF_UART2].hw.h_uart.hdmatx);
}



void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  drv_uart_t *p_drv_uart;
  uint8_t channel = drvUartGetIndex(huart);

  p_drv_uart = &drv_uart_tbl[channel];

  if (huart->Instance == USART1)
  {
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_USART1_CLK_ENABLE();

    // TX
    GPIO_InitStruct.Pin       = GPIO_PIN_6;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // RX
    GPIO_InitStruct.Pin       = GPIO_PIN_7;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Enable DMA clock */
    __HAL_RCC_DMA2_CLK_ENABLE();


    if (p_drv_uart->hw.tx_dma_enable == true)
    {
      /* Configure the DMA handler for Transmission process */
      p_drv_uart->hw.hdma_tx.Instance                 = DMA2_Stream7;
      p_drv_uart->hw.hdma_tx.Init.Channel             = DMA_CHANNEL_4;
      p_drv_uart->hw.hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
      p_drv_uart->hw.hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
      p_drv_uart->hw.hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
      p_drv_uart->hw.hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
      p_drv_uart->hw.hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
      p_drv_uart->hw.hdma_tx.Init.Mode                = DMA_NORMAL;
      p_drv_uart->hw.hdma_tx.Init.Priority            = DMA_PRIORITY_LOW;
      p_drv_uart->hw.hdma_tx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
      p_drv_uart->hw.hdma_tx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
      p_drv_uart->hw.hdma_tx.Init.MemBurst            = DMA_MBURST_INC4;
      p_drv_uart->hw.hdma_tx.Init.PeriphBurst         = DMA_PBURST_INC4;

      HAL_DMA_Init(&p_drv_uart->hw.hdma_tx);

      /* Associate the initialized DMA handle to the the UART handle */
      __HAL_LINKDMA((&p_drv_uart->hw.h_uart), hdmatx, (p_drv_uart->hw.hdma_tx));
    }

    /* Configure the DMA handler for reception process */
    p_drv_uart->hw.hdma_rx.Instance                 = DMA2_Stream5;
    p_drv_uart->hw.hdma_rx.Init.Channel             = DMA_CHANNEL_4;
    p_drv_uart->hw.hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    p_drv_uart->hw.hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
    p_drv_uart->hw.hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
    p_drv_uart->hw.hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    p_drv_uart->hw.hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    p_drv_uart->hw.hdma_rx.Init.Mode                = DMA_CIRCULAR;
    p_drv_uart->hw.hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;
    p_drv_uart->hw.hdma_rx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
    p_drv_uart->hw.hdma_rx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
    p_drv_uart->hw.hdma_rx.Init.MemBurst            = DMA_MBURST_INC4;
    p_drv_uart->hw.hdma_rx.Init.PeriphBurst         = DMA_PBURST_INC4;

    HAL_DMA_Init(&p_drv_uart->hw.hdma_rx);

    /* Associate the initialized DMA handle to the the UART handle */
    __HAL_LINKDMA((&p_drv_uart->hw.h_uart), hdmarx, (p_drv_uart->hw.hdma_rx));

    /*##-4- Configure the NVIC for DMA #########################################*/
    HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);

    HAL_NVIC_SetPriority(DMA2_Stream5_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(DMA2_Stream5_IRQn);

    HAL_NVIC_SetPriority(USART1_IRQn, 4, 1);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
  {
    __HAL_RCC_USART1_FORCE_RESET();
    __HAL_RCC_USART1_RELEASE_RESET();

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_7);
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6);

    HAL_NVIC_DisableIRQ(USART1_IRQn);
    HAL_NVIC_DisableIRQ(DMA2_Stream5_IRQn);
    HAL_NVIC_DisableIRQ(DMA2_Stream7_IRQn);
  }
}

