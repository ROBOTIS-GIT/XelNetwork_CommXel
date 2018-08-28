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


//-- External Variables
//

//-- Internal Functions
static void threadCmdIf(void const * argument);
static void threadButtonLed(void const * argument);
static void threadROS2(void const * argument);

//-- External Functions


void apInit(void)
{
  osThreadId ret;

  /* Setup swtimer */
  timerSetPeriod(_DEF_TIMER1, 1000);
  timerAttachInterrupt(_DEF_TIMER1, swtimerISR);
  timerStart(_DEF_TIMER1);

  cmdifBegin(_DEF_UART1, 57600);
  dxlportOpen(_DEF_DXL1, 1000000);

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

  osThreadDef(threadCmdIf, threadCmdIf, osPriorityNormal, 0, 8*1024/4);
  ret = osThreadCreate (osThread(threadCmdIf), NULL);
  if (ret == NULL)
  {
    cmdifPrintf("osThreadCreate : threadCmdIf fail\n");
  }

  osThreadDef(threadButtonLed, threadButtonLed, osPriorityNormal, 0, 1*1024/4);
  ret = osThreadCreate (osThread(threadButtonLed), NULL);
  if (ret == NULL)
  {
    cmdifPrintf("osThreadCreate : threadButtonLed fail\n");
  }

  osThreadDef(threadROS2, threadROS2, osPriorityNormal, 0, 32*1024/4);
  ret = osThreadCreate (osThread(threadROS2), NULL);
  if (ret == NULL)
  {
    cmdifPrintf("osThreadCreate : threadROS2 fail\n");
  }

  xelsInit();

  osKernelStart();
}


void apMain(void)
{
  while(1){

  }
}


#include "xel_net/xel_net.hpp"

static void threadROS2(void const * argument)
{
  for(;;)
  {
    if(ethernetGetDhcpStatus() == DHCP_ADDRESS_ASSIGNED)
    {
      break;
    }
  }

  ros2::init("192.168.60.88", 2018);
  XelNetwork::XelNetworkNode XelNetNode;


  for( ;; )
  {
    ros2::spin(&XelNetNode);
  }


  for( ;; )
  {
    osThreadTerminate(NULL);
  }
}




static void threadCmdIf(void const * argument)
{
  //cmdifPrintf("CMDEV-FW \n");

  for( ;; )
  {
    cmdifMain();
    delay(1);
  }
}

static void threadButtonLed(void const * argument)
{
  delay(500);

  for(;;)
  {
    if (buttonGetPressedEvent(_DEF_BUTTON1))
    {
      ledOn(_DEF_LED1);
    }
    else if (buttonGetReleasedEvent(_DEF_BUTTON1))
    {
      ledOff(_DEF_LED1);
    }
    delay(1);
  }
}
