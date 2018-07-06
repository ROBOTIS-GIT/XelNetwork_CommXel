/*
 * bsp.h
 *
 *  Created on: Feb 10, 2017
 *      Author: baram
 */

#ifndef BSP_H_
#define BSP_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdbool.h>

#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"

#include "hw_def.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_cdc.h"
#include "usbd_cdc_interface.h"

#define _BSP_DEF_BOARD_NAME "CM-DEV revA"
#define _BSP_BOARD_VER_NUM  1

void bspInit();
void bspDeinit(void);
void bspInitUSB(void);
void bspJumpToAddress(uint32_t address);

#ifdef __cplusplus
 }
#endif
#endif /* BSP_H_ */
