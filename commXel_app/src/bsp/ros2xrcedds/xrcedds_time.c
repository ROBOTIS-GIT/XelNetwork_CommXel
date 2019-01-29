/*
 * xrcedds_time.c
 *
 *  Created on: Jan 29, 2019
 *      Author: kei
 */

#include <stdint.h>
#include "hw.h"

uint32_t dds_getMilliseconds(void)
{
  return millis();
}
