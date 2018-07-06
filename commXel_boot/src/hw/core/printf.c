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
  vcpPutch(ch);

  return 1;
}

int __io_getchar(void)
{
  return vcpGetch();
}



