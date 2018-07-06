/*
 * ap.cpp
 *
 *  Created on: 2017. 2. 13.
 *      Author: baram
 */

#include "ap.h"
#include "hw.h"
#include "ap_def.h"
//#include "test_def.h"



//-- External Variables
//

//-- Internal Functions

//-- External Functions
void apInit(void)
{
  /* Setup swtimer */
  timerSetPeriod(_DEF_TIMER1, 1000);
  timerAttachInterrupt(_DEF_TIMER1, swtimerISR);
  timerStart(_DEF_TIMER1);

  bootInit();
}

void apMain(void)
{
  while(1)
  {
    bootProcess();
  }
}

