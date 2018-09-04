/*
 * usb_to_dxl.cpp
 *
 *  Created on: Sep 3, 2018
 *      Author: kei
 */

#include "usb_to_dxl.hpp"

static uint8_t dxl_ch = _DEF_DXL1;

void u2dInit(void)
{
  dxlportInit();
  dxlportOpen(dxl_ch, 57600);
}

void u2dRun(void)
{
  uint32_t length, i;

  if(vcpGetBaud() != dxlportGetBaud(dxl_ch))
  {
    dxlportOpen(dxl_ch, vcpGetBaud());
  }

  // USB to DXL
  length = vcpAvailable();
  if( length > 0 )
  {
    uint8_t data;
    dxlportTxEnable(dxl_ch);
    for(i=0; i<length; i++ )
    {
      data = vcpRead();
      dxlportWrite(dxl_ch, &data, 1);
    }
    dxlportTxDisable(dxl_ch);
  }

  // DXL to USB
  length = dxlportAvailable(dxl_ch);
  if( length > 0 )
  {
    uint8_t tx_buffer[256];
    if(length > (uint32_t)256)
    {
      length = 256;
    }

    for(i=0; i<length; i++ )
    {
      tx_buffer[i] = dxlportRead(dxl_ch);
    }
    vcpWrite(tx_buffer, length);
  }
}

