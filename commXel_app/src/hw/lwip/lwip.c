/*
 * lwip.c
 *
 *  Created on: 2017. 10. 18.
 *      Author: opus
 */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "lwip.h"

#include "netif/etharp.h"


#ifdef _USE_HW_ETH

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** ETH GPIO Configuration
PA1     ------> ETH_REF_CLK  // 35
PC1     ------> ETH_MDC    // 27
PA2     ------> ETH_MDIO   // 36
PA7     ------> ETH_CRS_DV  // 43
PB11    ------> ETH_TX_EN // 70
PB12    ------> ETH_TXD0  // 73
PB13    ------> ETH_TXD1  // 74
PC4     ------> ETH_RXD0  // 44
PC5     ------> ETH_RXD1  // 45
PC0     ------> ETH_RXER  // 26 //GPIO RX ERROR
*/
#define ETH_REF_CLK_Pin GPIO_PIN_1
#define ETH_REF_CLK_GPIO_Port GPIOA
#define ETH_MDC_Pin GPIO_PIN_1
#define ETH_MDC_GPIO_Port GPIOC
#define ETH_MDIO_Pin GPIO_PIN_2
#define ETH_MDIO_GPIO_Port GPIOA
#define ETH_CRS_DV_Pin GPIO_PIN_7
#define ETH_CRS_DV_GPIO_Port GPIOA

#define ETH_TX_EN_Pin GPIO_PIN_11
#define ETH_TX_EN_GPIO_Port GPIOB
#define ETH_TXD0_Pin GPIO_PIN_12
#define ETH_TXD0_GPIO_Port GPIOB
#define ETH_TXD1_Pin GPIO_PIN_13
#define ETH_TXD1_GPIO_Port GPIOB

#define ETH_RXD0_Pin GPIO_PIN_4
#define ETH_RXD0_GPIO_Port GPIOC
#define ETH_RXD1_Pin GPIO_PIN_5
#define ETH_RXD1_GPIO_Port GPIOC
#define ETH_RXER_Pin GPIO_PIN_0
#define ETH_RXER_GPIO_Port GPIOC

#ifdef _USE_HW_RTOS
#define TIME_WAITING_FOR_INPUT                 ( osWaitForever )
#define INTERFACE_THREAD_STACK_SIZE            ( 350 )
#endif

// Define those to better describe your network interface.
#define IFNAME0 's'
#define IFNAME1 't'

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/*
  @Note: The DMARxDscrTab and DMATxDscrTab must be declared in a non cacheable memory region
         In this example they are declared in the first 256 Byte of SRAM1 memory, so this
         memory region is configured by MPU as a device memory (please refer to MPU_Config() in main.c).

         In this example the ETH buffers are located in the SRAM2 memory,
         since the data cache is enabled, so cache maintenance operations are mandatory.
 */

ETH_DMADescTypeDef  DMARxDscrTab[ETH_RXBUFNB] __attribute__((section(".RxDecripSection")));/* Ethernet Rx MA Descriptor */

ETH_DMADescTypeDef  DMATxDscrTab[ETH_TXBUFNB] __attribute__((section(".TxDescripSection")));/* Ethernet Tx DMA Descriptor */

uint8_t Rx_Buff[ETH_RXBUFNB][ETH_RX_BUF_SIZE] __attribute__((section(".RxarraySection"))); /* Ethernet Receive Buffer */

uint8_t Tx_Buff[ETH_TXBUFNB][ETH_TX_BUF_SIZE] __attribute__((section(".TxarraySection"))); /* Ethernet Transmit Buffer */


#ifdef _USE_HW_RTOS
// Semaphore to signal incoming packets
osSemaphoreId s_xSemaphore = NULL;
#endif

// Global Ethernet handle
ETH_HandleTypeDef EthHandle;

/* Private function prototypes -----------------------------------------------*/
static void lwipLowLevelHwInit(struct netif *netif);
static void lwipRecvTask( void const * argument );
static err_t lwipLowLevelSend(struct netif *netif, struct pbuf *p);
static struct pbuf * lwipLowLevelRecv(struct netif *netif);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief Should be called at the beginning of the program to set up the
  * network interface. It calls the function lwipLowLevelHwInit() to do the
  * actual setup of the hardware.
  *
  * This function should be passed as a parameter to netif_add().
  *
  * @param netif the lwip network interface structure
  * @return ERR_OK if the loopif is initialized
  *         ERR_MEM if private data couldn't be allocated
  *         any other err_t on error
  */
err_t lwipHwInit(struct netif *netif)
{
  LWIP_ASSERT("netif != NULL", (netif != NULL));

#if LWIP_NETIF_HOSTNAME
  /* Initialize interface hostname */
  netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

  netif->name[0] = IFNAME0;
  netif->name[1] = IFNAME1;

  /* We directly use etharp_output() here to save a function call.
   * You can instead declare your own function an call etharp_output()
   * from it if you have to do some checks before sending (e.g. if link
   * is available...) */
  netif->output = etharp_output;
  netif->linkoutput = lwipLowLevelSend;

  // initialize the hardware
  lwipLowLevelHwInit(netif);

  return ERR_OK;
}

/**
  * @brief  Returns the current time in milliseconds
  *         when LWIP_TIMERS == 1 and NO_SYS == 1
  * @param  None
  * @retval Time
  */
u32_t sys_now(void)
{
  return HAL_GetTick();
}

/*******************************************************************************
                       LL Driver Interface ( LwIP stack --> ETH)
*******************************************************************************/
/**
  * @brief In this function, the hardware should be initialized.
  * Called from ethInit().
  *
  * @param netif the already initialized lwip network interface structure
  */
static void lwipLowLevelHwInit(struct netif *netif)
{
  EthHandle.Instance = ETH;
  EthHandle.Init.MACAddr = netif->hwaddr;
  EthHandle.Init.AutoNegotiation = ETH_AUTONEGOTIATION_ENABLE;
  EthHandle.Init.Speed = ETH_SPEED_100M;
  EthHandle.Init.DuplexMode = ETH_MODE_FULLDUPLEX;
  EthHandle.Init.MediaInterface = ETH_MEDIA_INTERFACE_RMII;
  EthHandle.Init.RxMode = ETH_RXINTERRUPT_MODE;
  EthHandle.Init.ChecksumMode = ETH_CHECKSUM_BY_HARDWARE;
  EthHandle.Init.PhyAddress = LAN8742A_PHY_ADDRESS;

  // configure ethernet peripheral (GPIOs, clocks, MAC, DMA)
  if (HAL_ETH_Init(&EthHandle) == HAL_OK)
  {
    netif->flags |= NETIF_FLAG_LINK_UP;
  }

  // Initialize Tx Descriptors list: Chain Mode
  HAL_ETH_DMATxDescListInit(&EthHandle, DMATxDscrTab, &Tx_Buff[0][0], ETH_TXBUFNB);

  // Initialize Rx Descriptors list: Chain Mode
  HAL_ETH_DMARxDescListInit(&EthHandle, DMARxDscrTab, &Rx_Buff[0][0], ETH_RXBUFNB);

  // set netif MAC hardware address length
  netif->hwaddr_len = ETHARP_HWADDR_LEN;

  // set netif maximum transfer unit
  netif->mtu = 1500;

  // Accept broadcast address and ARP traffic
  netif->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_IGMP;

#ifdef _USE_HW_RTOS
  // create a binary semaphore used for informing ethernetif of frame reception
  osSemaphoreDef(SEM);
  s_xSemaphore = osSemaphoreCreate(osSemaphore(SEM) , 1 );

  // create the task that handles the ETH_MAC
  osThreadDef(EthIf, lwipRecvTask, osPriorityRealtime, 0, INTERFACE_THREAD_STACK_SIZE);
  osThreadCreate (osThread(EthIf), netif);
#endif

  // Enable MAC and DMA transmission and reception
  HAL_ETH_Start(&EthHandle);
}


/**
  * @brief This function should do the actual transmission of the packet. The packet is
  * contained in the pbuf that is passed to the function. This pbuf
  * might be chained.
  *
  * @param netif the lwip network interface structure for this ethernetif
  * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
  * @return ERR_OK if the packet could be sent
  *         an err_t value if the packet couldn't be sent
  *
  * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
  *       strange results. You might consider waiting for space in the DMA queue
  *       to become available since the stack doesn't retry to send a packet
  *       dropped because of memory failure (except for the TCP timers).
  */
static err_t lwipLowLevelSend(struct netif *netif, struct pbuf *p)
{
  err_t errval;
  struct pbuf *q;
  uint8_t *buffer = (uint8_t *)(EthHandle.TxDesc->Buffer1Addr);
  __IO ETH_DMADescTypeDef *DmaTxDesc;
  uint32_t framelength = 0;
  uint32_t bufferoffset = 0;
  uint32_t byteslefttocopy = 0;
  uint32_t payloadoffset = 0;

  DmaTxDesc = EthHandle.TxDesc;
  bufferoffset = 0;

  // copy frame from pbufs to driver buffers
  for(q = p; q != NULL; q = q->next)
  {
    /* Is this buffer available? If not, goto error */
    if((DmaTxDesc->Status & ETH_DMATXDESC_OWN) != (uint32_t)RESET)
    {
      errval = ERR_USE;
      goto error;
    }

    // Get bytes in current lwIP buffer
    byteslefttocopy = q->len;
    payloadoffset = 0;

    // Check if the length of data to copy is bigger than Tx buffer size
    while( (byteslefttocopy + bufferoffset) > ETH_TX_BUF_SIZE )
    {
      // Copy data to Tx buffer
      memcpy( (uint8_t*)((uint8_t*)buffer + bufferoffset), (uint8_t*)((uint8_t*)q->payload + payloadoffset), (ETH_TX_BUF_SIZE - bufferoffset) );

      // Point to next descriptor
      DmaTxDesc = (ETH_DMADescTypeDef *)(DmaTxDesc->Buffer2NextDescAddr);

      // Check if the buffer is available
      if((DmaTxDesc->Status & ETH_DMATXDESC_OWN) != (uint32_t)RESET)
      {
        errval = ERR_USE;
        goto error;
      }

      buffer = (uint8_t *)(DmaTxDesc->Buffer1Addr);

      byteslefttocopy = byteslefttocopy - (ETH_TX_BUF_SIZE - bufferoffset);
      payloadoffset = payloadoffset + (ETH_TX_BUF_SIZE - bufferoffset);
      framelength = framelength + (ETH_TX_BUF_SIZE - bufferoffset);
      bufferoffset = 0;
    }

    // Copy the remaining bytes
    memcpy( (uint8_t*)((uint8_t*)buffer + bufferoffset), (uint8_t*)((uint8_t*)q->payload + payloadoffset), byteslefttocopy );
    bufferoffset = bufferoffset + byteslefttocopy;
    framelength = framelength + byteslefttocopy;
  }

  /* Clean and Invalidate data cache */
  SCB_CleanInvalidateDCache();
  /* Prepare transmit descriptors to give to DMA */
  HAL_ETH_TransmitFrame(&EthHandle, framelength);

  errval = ERR_OK;

error:

  // When Transmit Underflow flag is set, clear it and issue a Transmit Poll Demand to resume transmission
  if ((EthHandle.Instance->DMASR & ETH_DMASR_TUS) != (uint32_t)RESET)
  {
    // Clear TUS ETHERNET DMA flag
    EthHandle.Instance->DMASR = ETH_DMASR_TUS;

    // Resume DMA transmission
    EthHandle.Instance->DMATPDR = 0;
  }
  return errval;
}

/**
  * @brief Should allocate a pbuf and transfer the bytes of the incoming
  * packet from the interface into the pbuf.
  *
  * @param netif the lwip network interface structure for this ethernetif
  * @return a pbuf filled with the received packet (including MAC header)
  *         NULL on memory error
  */
static struct pbuf * lwipLowLevelRecv(struct netif *netif)
{
  struct pbuf *p = NULL, *q = NULL;
  uint16_t len = 0;
  uint8_t *buffer;
  __IO ETH_DMADescTypeDef *dmarxdesc;
  uint32_t bufferoffset = 0;
  uint32_t payloadoffset = 0;
  uint32_t byteslefttocopy = 0;
  uint32_t i=0;

  // get received frame
  if(HAL_ETH_GetReceivedFrame_IT(&EthHandle) != HAL_OK)
    return NULL;

  // Obtain the size of the packet and put it into the "len" variable.
  len = EthHandle.RxFrameInfos.length;
  buffer = (uint8_t *)EthHandle.RxFrameInfos.buffer;

  if (len > 0)
  {
    // We allocate a pbuf chain of pbufs from the Lwip buffer pool
    p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);
  }

  // Clean and Invalidate data cache
  SCB_CleanInvalidateDCache();

  if (p != NULL)
  {
    dmarxdesc = EthHandle.RxFrameInfos.FSRxDesc;
    bufferoffset = 0;

    for(q = p; q != NULL; q = q->next)
    {
      byteslefttocopy = q->len;
      payloadoffset = 0;

      // Check if the length of bytes to copy in current pbuf is bigger than Rx buffer size
      while( (byteslefttocopy + bufferoffset) > ETH_RX_BUF_SIZE )
      {
        // Copy data to pbuf
        memcpy( (uint8_t*)((uint8_t*)q->payload + payloadoffset), (uint8_t*)((uint8_t*)buffer + bufferoffset), (ETH_RX_BUF_SIZE - bufferoffset));

        // Point to next descriptor
        dmarxdesc = (ETH_DMADescTypeDef *)(dmarxdesc->Buffer2NextDescAddr);
        buffer = (uint8_t *)(dmarxdesc->Buffer1Addr);

        byteslefttocopy = byteslefttocopy - (ETH_RX_BUF_SIZE - bufferoffset);
        payloadoffset = payloadoffset + (ETH_RX_BUF_SIZE - bufferoffset);
        bufferoffset = 0;
      }

      // Copy remaining data in pbuf
      memcpy( (uint8_t*)((uint8_t*)q->payload + payloadoffset), (uint8_t*)((uint8_t*)buffer + bufferoffset), byteslefttocopy);
      bufferoffset = bufferoffset + byteslefttocopy;
    }
  }

  /* Release descriptors to DMA
     Point to first descriptor */
  dmarxdesc = EthHandle.RxFrameInfos.FSRxDesc;
  /* Set Own bit in Rx descriptors: gives the buffers back to DMA */
  for (i=0; i< EthHandle.RxFrameInfos.SegCount; i++)
  {
    dmarxdesc->Status |= ETH_DMARXDESC_OWN;
    dmarxdesc = (ETH_DMADescTypeDef *)(dmarxdesc->Buffer2NextDescAddr);
  }

  // Clear Segment_Count */
  EthHandle.RxFrameInfos.SegCount =0;

  // When Rx Buffer unavailable flag is set: clear it and resume reception
  if ((EthHandle.Instance->DMASR & ETH_DMASR_RBUS) != (uint32_t)RESET)
  {
    // Clear RBUS ETHERNET DMA flag
    EthHandle.Instance->DMASR = ETH_DMASR_RBUS;
    // Resume DMA reception
    EthHandle.Instance->DMARPDR = 0;
  }
  return p;
}

#ifdef _USE_HW_RTOS
/**
  * @brief This function is the lwipRecvTask task, it is processed when a packet
  * is ready to be read from the interface. It uses the function lwipLowLevelRecv()
  * that should handle the actual reception of bytes from the network
  * interface. Then the type of the received packet is determined and
  * the appropriate input function is called.
  *
  * @param netif the lwip network interface structure
  */
static void lwipRecvTask( void const * argument )
{
  struct pbuf *p;
  struct netif *netif = (struct netif *) argument;

  for( ;; )
  {
    if (osSemaphoreWait( s_xSemaphore, TIME_WAITING_FOR_INPUT)==osOK)
    {
      do
      {
        p = lwipLowLevelRecv( netif );
        if (p != NULL)
        {
          if (netif->input( p, netif) != ERR_OK )
          {
            pbuf_free(p);
          }
        }
      }while(p!=NULL);
    }
  }
}
#endif

/*******************************************************************************
                       Ethernet MSP Routines
*******************************************************************************/
void HAL_ETH_MspInit(ETH_HandleTypeDef* ethHandle)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  if(ethHandle->Instance==ETH)
  {
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Alternate = GPIO_AF11_ETH;

    GPIO_InitStruct.Pin = ETH_REF_CLK_Pin;
    HAL_GPIO_Init(ETH_REF_CLK_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = ETH_MDC_Pin;
    HAL_GPIO_Init(ETH_MDC_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = ETH_MDIO_Pin;
    HAL_GPIO_Init(ETH_MDIO_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = ETH_CRS_DV_Pin;
    HAL_GPIO_Init(ETH_CRS_DV_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = ETH_RXD0_Pin;
    HAL_GPIO_Init(ETH_RXD0_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = ETH_RXD1_Pin;
    HAL_GPIO_Init(ETH_RXD1_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = ETH_RXER_Pin;
    HAL_GPIO_Init(ETH_RXER_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = ETH_TX_EN_Pin;
    HAL_GPIO_Init(ETH_TX_EN_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = ETH_TXD0_Pin;
    HAL_GPIO_Init(ETH_TXD0_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = ETH_TXD1_Pin;
    HAL_GPIO_Init(ETH_TXD1_GPIO_Port, &GPIO_InitStruct);

    // Enable the Ethernet global Interrupt
    HAL_NVIC_SetPriority(ETH_IRQn, 0x7, 0);
    HAL_NVIC_EnableIRQ(ETH_IRQn);

    __HAL_RCC_ETH_CLK_ENABLE();
  }
}

void HAL_ETH_MspDeInit(ETH_HandleTypeDef* ethHandle)
{
  if(ethHandle->Instance==ETH)
  {
    __HAL_RCC_ETH_CLK_DISABLE();

    HAL_GPIO_DeInit(ETH_REF_CLK_GPIO_Port, ETH_REF_CLK_Pin);
    HAL_GPIO_DeInit(ETH_MDC_GPIO_Port, ETH_MDC_Pin);
    HAL_GPIO_DeInit(ETH_MDIO_GPIO_Port, ETH_MDIO_Pin);
    HAL_GPIO_DeInit(ETH_CRS_DV_GPIO_Port, ETH_CRS_DV_Pin);
    HAL_GPIO_DeInit(ETH_RXD0_GPIO_Port, ETH_RXD0_Pin);
    HAL_GPIO_DeInit(ETH_RXD1_GPIO_Port, ETH_RXD1_Pin);
    HAL_GPIO_DeInit(ETH_RXER_GPIO_Port, ETH_RXER_Pin);
    HAL_GPIO_DeInit(ETH_TX_EN_GPIO_Port, ETH_TX_EN_Pin);
    HAL_GPIO_DeInit(ETH_TXD0_GPIO_Port, ETH_TXD0_Pin);
    HAL_GPIO_DeInit(ETH_TXD1_GPIO_Port, ETH_TXD1_Pin);

    HAL_NVIC_DisableIRQ(ETH_IRQn);
  }
}


/**
  * @brief  Ethernet Rx Transfer completed callback
  * @param  heth: ETH handle
  * @retval None
  */
void HAL_ETH_RxCpltCallback(ETH_HandleTypeDef *heth)
{
#ifdef _USE_HW_RTOS
  osSemaphoreRelease(s_xSemaphore);
#endif
}


#endif
