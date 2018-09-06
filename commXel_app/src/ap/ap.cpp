/*
 * ap.cpp
 *
 *  Created on: 2017. 2. 13.
 *      Author: baram
 */





#include "ap.hpp"
#include "hw.h"

//-- Internal Variables
//
const volatile  __attribute__((section(".version_str"))) uint8_t fw_version_str[256] = _DEF_APP_VER_STR;
const volatile  __attribute__((section(".version_num"))) uint8_t fw_version_num[256] = _DEF_APP_VER_NUM;

static ap_t ap_log;

ap_t *p_ap = &ap_log;


//-- External Variables
//
osSemaphoreId dxl_semaphore;

//-- Internal Functions
static void threadApMode(void const * argument);
static void threadXelNetwork(void const * argument);
static void threadXelPlugAndPlay(void const * argument);
static void threadUsbDxlBypass(void const * argument);

//-- External Functions

void apInit(void)
{
  osThreadId ret;

  /* Setup swtimer */
  timerSetPeriod(_DEF_TIMER1, 1000);
  timerAttachInterrupt(_DEF_TIMER1, swtimerISR);
  timerStart(_DEF_TIMER1);

  cmdifBegin(_DEF_UART1, 57600);
  //dxlportOpen(_DEF_DXL1, 1000000);


  p_ap->model_number = DXL_MODEL_NUMBER;
  p_ap->firmware_version = 1;

  p_ap->dxl_slave.use  = true;
  p_ap->dxl_slave.ch   = _DEF_DXL2;
  p_ap->dxl_slave.id   = DXL_INIT_ID;
  p_ap->dxl_slave.baud = 1000000;

  p_ap->p_dxl_usb  = &p_ap->dxl_slave;

  xelsInit();

  dxlSlaveInit();
  dxlCtableInit();



  osSemaphoreDef(semaphore_dxl);
  dxl_semaphore = osSemaphoreCreate(osSemaphore(semaphore_dxl) , 1);

  /* Begin Ethernet */
#ifdef _USE_HW_ETH
  static uint8_t mac_addr[6] = _HW_DEF_DEFAULT_MAC_ADDR;
  static ip_addr_t ip_addr, subnet, gateway, dns_server;

  /* Static */
  IP4_ADDR(&ip_addr, 192, 168, 0, 4);
  IP4_ADDR(&subnet, 255, 255, 255, 0);
  IP4_ADDR(&gateway, 192, 168, 0, 1);
  IP4_ADDR(&dns_server, 8, 8, 8, 8);

  ethernetIfBegin(IP_DHCP, mac_addr, &ip_addr, &subnet, &gateway, &dns_server);
#endif /* _USE_HW_ETH */

  osThreadDef(threadApMode, threadApMode, osPriorityNormal, 0, 1*1024/4);
  ret = osThreadCreate (osThread(threadApMode), NULL);
  if (ret == NULL)
  {
    cmdifPrintf("osThreadCreate : threadApMode fail\n");
  }

  osThreadDef(threadXelPlugAndPlay, threadXelPlugAndPlay, osPriorityNormal, 0, 8*1024/4);
  ret = osThreadCreate (osThread(threadXelPlugAndPlay), NULL);
  if (ret == NULL)
  {
    cmdifPrintf("osThreadCreate : threadXelPlugAndPlay fail\n");
  }

  osThreadDef(threadXelNetwork, threadXelNetwork, osPriorityNormal, 0, 32*1024/4);
  ret = osThreadCreate (osThread(threadXelNetwork), NULL);
  if (ret == NULL)
  {
    cmdifPrintf("osThreadCreate : threadXelNetwork fail\n");
  }

  osThreadDef(threadUsbDxlBypass, threadUsbDxlBypass, osPriorityNormal, 0, 8*1024/4);
  ret = osThreadCreate (osThread(threadUsbDxlBypass), NULL);
  if (ret == NULL)
  {
    cmdifPrintf("osThreadCreate : threadUsbDxlBypass fail\n");
  }

  osKernelStart();
}


void apMain(void)
{
  while(1){

  }
}


enum OperatingMode
{
  OP_USB_DXL_BYPASS = 0,
  OP_XEL_NETWORK,
  OP_CONTROL_TABLE
};

static OperatingMode operating_mode = OP_XEL_NETWORK;

static void threadApMode(void const * argument)
{
  for(;;)
  {
    if (buttonGetReleasedEvent(_HW_DEF_BUTTON_MODE) == true)
    {
      if (buttonGetReleasedTime(_HW_DEF_BUTTON_MODE) < 500
          && buttonGetPressedTime(_HW_DEF_BUTTON_MODE) > 2000 && buttonGetPressedTime(_HW_DEF_BUTTON_MODE) < 5000)
      {
        if(operating_mode == OP_XEL_NETWORK)
        {
          operating_mode = OP_USB_DXL_BYPASS;
          ledOn(_DEF_LED1);
        }
        else
        {
          operating_mode = OP_XEL_NETWORK;
          ledOff(_DEF_LED1);
        }
      }
      else if (buttonGetReleasedTime(_HW_DEF_BUTTON_MODE) < 500
          && buttonGetPressedTime(_HW_DEF_BUTTON_MODE) > 5000)
      {
        operating_mode = OP_CONTROL_TABLE;
      }
    }
    osThreadYield();
  }
}

static void threadXelNetwork(void const * argument)
{
  for(;;)
  {
    if(ethernetGetDhcpStatus() == DHCP_ADDRESS_ASSIGNED)
    {
      char ip_addr[16];
      strcpy(ip_addr, ethernetGetIpAddrAsString());
      for(uint8_t i = 0; i < (uint8_t)strlen(ip_addr); i++)
      {
        eepromWriteByte(P_ASSIGNED_IP+i, (uint8_t)ip_addr[i]);
      }
      break;
    }
  }

  ros2::init("192.168.60.88", 2018);
  XelNetwork::Core XelNetwork;

  for( ;; )
  {
    if(operating_mode == OP_XEL_NETWORK)
    {
      XelNetwork.run();
    }
    osThreadYield();
  }

  for( ;; )
  {
    osThreadTerminate(NULL);
  }
}

static void threadXelPlugAndPlay(void const * argument)
{
  XelNetwork::PlugAndPlay PlugAndPlay(100);

  for( ;; )
  {
    if(operating_mode == OP_XEL_NETWORK)
    {
      PlugAndPlay.run();
    }
    osThreadYield();
  }

  for( ;; )
  {
    osThreadTerminate(NULL);
  }
}

static void threadUsbDxlBypass(void const * argument)
{
  for( ;; )
  {
    switch(operating_mode)
    {
      case OP_USB_DXL_BYPASS:
        u2dRun();
        break;

      case OP_CONTROL_TABLE:
        dxlSlaveLoop();
        break;

      default:
        break;
    }
    osThreadYield();
  }

  for( ;; )
  {
    osThreadTerminate(NULL);
  }
}
