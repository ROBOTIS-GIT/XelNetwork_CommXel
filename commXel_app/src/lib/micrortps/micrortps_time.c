/*
 * micrortps_time.c
 *
 *  Created on: Jul 31, 2018
 *      Author: kei
 */

#include <micrortps/client/core/util/time.h>
#include <time.h>
#include "hw.h"

#if 1
int64_t clockGetTime(void)
{
  static uint32_t pre_msec = 0, now_msec;
  static uint64_t msec = 0;

  now_msec = millis();
  msec += (uint64_t) (now_msec - pre_msec);
  pre_msec = now_msec;

  return msec;
}
#else
int clock_gettime(clockid_t clk_id, struct timespec *tp)
{
  (void)(clk_id);

  static uint32_t pre_msec = 0, now_msec;
  static struct timespec system_time = {0, 0};

  now_msec = millis();
  system_time.tv_sec  += (time_t)((now_msec - pre_msec)/1000);
  system_time.tv_nsec  = (long)(((now_msec - pre_msec)%1000)*1000000UL);
  pre_msec = now_msec;

  tp->tv_sec  = system_time.tv_sec;
  tp->tv_nsec = system_time.tv_nsec;

  return 0;
}
#endif


