/*
 * usb_to_dxl.cpp
 *
 *  Created on: Sep 3, 2018
 *      Author: kei
 */

#include "usb_to_dxl.hpp"

static uint8_t dxl_ch = _DEF_DXL1;

static uint32_t tx_time_total = 0, rx_time_total = 0, tx_length_total = 0, rx_length_total = 0;

void u2dInit(void)
{
  dxlportInit();
  dxlportOpen(dxl_ch, 57600);
}

static uint8_t tx_buffer[256];

void u2dRun(void)
{
  uint32_t length, i;
  uint32_t pre_time;
  uint32_t usb_baud, dxl_baud;

  usb_baud = vcpGetBaud();
  dxl_baud = dxlportGetBaud(dxl_ch);

  if(usb_baud != dxl_baud)
  {
    dxlportOpen(dxl_ch, usb_baud);
    tx_time_total = 0;
    tx_length_total = 0;
    rx_time_total = 0;
    rx_length_total = 0;
  }

  // USB to DXL
  length = vcpAvailable();
  if( length > 0 )
  {
    if(length > (uint32_t)256)
    {
      length = 256;
    }

    for(i=0; i<length; i++ )
    {
      tx_buffer[i] = vcpRead();
    }

    pre_time = micros();
    dxlportWrite(dxl_ch, tx_buffer, length);
    tx_time_total += (micros() - pre_time);
    tx_length_total += length;
  }

  // DXL to USB
  length = dxlportAvailable(dxl_ch);
  if( length > 0 )
  {

    if(length > (uint32_t)256)
    {
      length = 256;
    }

    pre_time = micros();
    for(i=0; i<length; i++ )
    {
      tx_buffer[i] = dxlportRead(dxl_ch);
    }
    rx_time_total += (micros() - pre_time);
    rx_length_total += length;

    vcpWrite(tx_buffer, length);
  }
}


void u2dPrintDebugMessages(uint32_t interval_ms)
{
  static uint32_t pre_time = 0;
  static char buf[256];

  if(millis() - pre_time >= interval_ms)
  {
    pre_time = millis();

    sprintf(buf, "[<< TX] %u (ns/Byte)\r\n[>> RX] %u (ns/Byte)\r\n",
        (unsigned int)(tx_time_total*1000/tx_length_total), (unsigned int)(rx_time_total*1000/rx_length_total));
    uartWrite(_DEF_UART3, (uint8_t*) buf, strlen(buf));
  }
}



