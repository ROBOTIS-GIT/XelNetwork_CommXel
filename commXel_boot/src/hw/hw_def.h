/*
 *  hw_def.h
 *
 *  Created on: 2016. 5. 14.
 *      Author: Baram
 */





#ifndef HW_DEF_H
#define HW_DEF_H

#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>

#include "def.h"
#include "error_code.h"


#ifndef BOOL
#define BOOL uint8_t
#endif

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define LEFT  1
#define RIGHT 0


#define _USE_HW_VCP
#define _USE_HW_LED
#define _USE_HW_BUTTON
#define _USE_HW_UART
#define _USE_HW_FLASH
#define _USE_HW_RESET
#define _USE_HW_TIMER
#define _USE_HW_SW_TIMER


#define _HW_DEF_DXL_MAX_BUFFER 1

//#define _USE_HW_CMDIF_LED
//#define _USE_HW_CMDIF_BUTTON
//#define _USE_HW_CMDIF_EEPROM
//#define _USE_HW_CMDIF_FLASH
//#define _USE_HW_CMDIF_BUZZER
//#define _USE_HW_CMDIF_RESET




#define _HW_DEF_CMDIF_LIST_MAX            16

#define _HW_DEF_BUTTON_CH_MAX             1
#define _HW_DEF_LED_CH_MAX                1
#define _HW_DEF_UART_CH_MAX               2
#define _HW_DEF_TIMER_CH_MAX              2
#define _HW_DEF_SW_TIMER_MAX              8

#define _HW_DEF_FLASH_ADDR_BOOT_VER_STR       0x08000600
#define _HW_DEF_FLASH_ADDR_BOOT_VER_NUM       0x08000700
#define _HW_DEF_FLASH_ADDR_APP_VER_STR        0x08040600
#define _HW_DEF_FLASH_ADDR_APP_VER_NUM        0x08040700

#define _HW_DEF_FLASH_ADDR_APP_LENGTH         (512*1024)
#define _HW_DEF_FLASH_ADDR_APP_START          0x08040000
#define _HW_DEF_FLASH_ADDR_APP_END            (_HW_DEF_FLASH_ADDR_APP_START + _HW_DEF_FLASH_ADDR_APP_LENGTH)

#define _HW_DEF_CMD_MAX_DATA_LENGTH           2048



#endif

