/*
 * vcp.c
 *
 *  Created on: 2017. 2. 13.
 *      Author: baram
 */



#include <stdarg.h>
#include <stdbool.h>

#include "hw.h"
#include "vcp.h"
#include "printf.h"





//-- Internal Variables
//


//-- External Variables
//


//-- Internal Functions
//



//-- External Functions
//





int __io_putchar(int ch)
{
  //vcpPutch(ch);

  uartPutch(_DEF_UART3, (uint8_t)ch);

  return 1;
}

int __io_getchar(void)
{
  //return vcpGetch();
  return uartGetch(_DEF_UART3);
  //return 0;
}



