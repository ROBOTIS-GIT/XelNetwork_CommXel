/*
 * ap.cpp
 *
 *  Created on: 2017. 2. 13.
 *      Author: baram, Kei
 */





#include "ap.hpp"
#include "hw.h"
#include "test/test.h"

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

  //cmdifBegin(_DEF_UART1, 57600);
  //dxlportOpen(_DEF_DXL1, 1000000);

  /* Setup DEBUG port */
  uartOpen(_DEF_UART3, 57600);

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
  static ip_addr_t ip_addr, subnet, gateway, dns_server;

  /* Static */
  IP4_ADDR(&ip_addr, 192, 168, 0, 44);
  IP4_ADDR(&subnet, 255, 255, 255, 0);
  IP4_ADDR(&gateway, 192, 168, 0, 1);
  IP4_ADDR(&dns_server, 8, 8, 8, 8);

  if(p_ap->mac_addr[0] != 0x00 || p_ap->mac_addr[1] != 0x00 || p_ap->mac_addr[2] != 0x00
      || p_ap->mac_addr[3] != 0x00 || p_ap->mac_addr[4] != 0x00 || p_ap->mac_addr[5] != 0x00 )
  {
    ethernetIfBegin((ip_assign_type_t)p_ap->dhcp_enable, p_ap->mac_addr, &ip_addr, &subnet, &gateway, &dns_server);
  }

#endif /* _USE_HW_ETH */

  osThreadDef(threadApMode, threadApMode, osPriorityNormal, 0, 1*1024/4);
  ret = osThreadCreate (osThread(threadApMode), NULL);
  if (ret == NULL)
  {
    cmdifPrintf("osThreadCreate : threadApMode fail\n");
  }

  osThreadDef(threadXelPlugAndPlay, threadXelPlugAndPlay, osPriorityNormal, 0, 4*1024/4);
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

  osThreadDef(threadUsbDxlBypass, threadUsbDxlBypass, osPriorityNormal, 0, 4*1024/4);
  ret = osThreadCreate (osThread(threadUsbDxlBypass), NULL);
  if (ret == NULL)
  {
    cmdifPrintf("osThreadCreate : threadUsbDxlBypass fail\n");
  }

  osKernelStart();
}


void apMain(void)
{
  while(1)
  {
  }
}

enum OperatingMode
{
  OP_USB_DXL_BYPASS = 0,
  OP_XEL_NETWORK,
  OP_CONTROL_TABLE,
  OP_TEST
};

static OperatingMode operating_mode = OP_XEL_NETWORK;
extern bool enable_auto_reset;

static void threadApMode(void const * argument)
{
  for(;;)
  {
    static uint32_t pre_time = 0;
    switch(operating_mode)
    {
      case OP_XEL_NETWORK:
        if(millis() - pre_time > 2000)
        {
          pre_time = millis();
          ledToggle(_DEF_LED1);
        }
        break;

      case OP_USB_DXL_BYPASS:
        if(millis() - pre_time > 500)
        {
          pre_time = millis();
          ledToggle(_DEF_LED1);
        }
        break;

      case OP_CONTROL_TABLE:
        if(millis() - pre_time > 50)
        {
          pre_time = millis();
          ledToggle(_DEF_LED1);
        }

      case OP_TEST:
        if(millis() - pre_time > 1000)
        {
          pre_time = millis();
          ledToggle(_DEF_LED1);
        }
        break;
    }

    if (buttonGetReleasedEvent(_HW_DEF_BUTTON_MODE) == true)
    {
      if (buttonGetReleasedTime(_HW_DEF_BUTTON_MODE) < 500
          && buttonGetPressedTime(_HW_DEF_BUTTON_MODE) > 1000 && buttonGetPressedTime(_HW_DEF_BUTTON_MODE) < 3000)
      {
        operating_mode = OP_USB_DXL_BYPASS;
        enable_auto_reset = false;
      }
      else if (buttonGetReleasedTime(_HW_DEF_BUTTON_MODE) < 500
          && buttonGetPressedTime(_HW_DEF_BUTTON_MODE) > 3000 && buttonGetPressedTime(_HW_DEF_BUTTON_MODE) < 5000)
      {
        operating_mode = OP_CONTROL_TABLE;
        enable_auto_reset = true;
      }
      else if (buttonGetReleasedTime(_HW_DEF_BUTTON_MODE) < 500
          && buttonGetPressedTime(_HW_DEF_BUTTON_MODE) > 5000)
      {
        operating_mode = OP_TEST;
        enable_auto_reset = true;
      }
      else
      {
        operating_mode = OP_XEL_NETWORK;
        enable_auto_reset = true;
      }
    }
    osThreadYield();
  }
}

static void threadXelNetwork(void const * argument)
{
  for(;;)
  {
    if(operating_mode == OP_XEL_NETWORK)
    {
      if(ethernetGetDhcpStatus() == DHCP_ADDRESS_ASSIGNED)
      {
        strcpy(p_ap->assigned_ip, ethernetGetIpAddrAsString());
        break;
      }
    }
    osThreadYield();
  }

  ros2::init(p_ap->remote_ip, p_ap->remote_port);
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
  XelNetwork::PlugAndPlay PlugAndPlay(300);

  for( ;; )
  {
    if(operating_mode == OP_XEL_NETWORK)
    {
      PlugAndPlay.run();
    }
    else if(operating_mode == OP_USB_DXL_BYPASS)
    {
      //u2dPrintDebugMessages(1000);
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
        if(osSemaphoreWait(dxl_semaphore, 0) == osOK)
        {
          u2dRun();
          osSemaphoreRelease(dxl_semaphore);
        }
        break;

      case OP_CONTROL_TABLE:
        if(osSemaphoreWait(dxl_semaphore, 0) == osOK)
        {
          dxlSlaveLoop();
          osSemaphoreRelease(dxl_semaphore);
        }
        break;

      case OP_TEST:
        testMain();
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
