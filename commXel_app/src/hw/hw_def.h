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



#define _USE_HW_RTOS
#define _USE_HW_LED
//#define _USE_HW_GPIO
//#define _USE_HW_EXTI
#define _USE_HW_VCP
#define _USE_HW_ETH
#define _USE_HW_BUTTON
#define _USE_HW_UART
#define _USE_HW_EEPROM
#define _USE_HW_FLASH
#define _USE_HW_RESET

#define _USE_HW_DXL
#define _USE_HW_DXLPORT
#define _USE_HW_DXLCMD

#define _USE_HW_CTABLE

#define _USE_HW_TIMER
#define _USE_HW_SW_TIMER

//#define _USE_HW_CMDIF_GPIO
//#define _USE_HW_CMDIF_EXTI
//#define _USE_HW_CMDIF_LED
//#define _USE_HW_CMDIF_BUTTON
//#define _USE_HW_CMDIF_EEPROM
//#define _USE_HW_CMDIF_FLASH
//#define _USE_HW_CMDIF_RESET
#define _USE_HW_CMDIF_DXLCMD

#define _HW_DEF_CMDIF_LIST_MAX                32
#define _HW_DEF_LED_CH_MAX                    1
#define _HW_DEF_SW_TIMER_MAX                  8
#define _HW_DEF_USER_EXTI_MAX                 1
#define _HW_DEF_UART_CH_MAX                   2
#define _HW_DEF_TIMER_CH_MAX                  2
#define _HW_DEF_BUTTON_CH_MAX                 1
#define _HW_DEF_DXLPORT_CH_MAX                2


#define _HW_DEF_DXLCMD_MAX_NODE               32
#define _HW_DEF_DXLCMD_MAX_NODE_BUFFER_SIZE   32
#define _HW_DEF_DXLCMD_BUFFER_MAX             1024
#define _HW_DEF_DXL_MAX_BUFFER                (1024+10)

#define _HW_DEF_BUTTON_MODE                   _DEF_BUTTON1



#define _HW_DEF_FLASH_ADDR_BOOT_VER_STR       0x08000600
#define _HW_DEF_FLASH_ADDR_BOOT_VER_NUM       0x08000700
#define _HW_DEF_FLASH_ADDR_APP_VER_STR        0x08040600
#define _HW_DEF_FLASH_ADDR_APP_VER_NUM        0x08040700

#define _HW_DEF_FLASH_ADDR_MOTION_LENGTH      (256*1024)
#define _HW_DEF_FLASH_ADDR_MOTION_START       0x080C0000
#define _HW_DEF_FLASH_ADDR_MOTION_END         (_HW_DEF_FLASH_ADDR_MOTION_START + _HW_DEF_FLASH_ADDR_MOTION_LENGTH)

#define _HW_DEF_FLASH_ADDR_TASK_LENGTH        (128*1024)
#define _HW_DEF_FLASH_ADDR_TASK_START         0x08030000
#define _HW_DEF_FLASH_ADDR_TASK_END           (_HW_DEF_FLASH_ADDR_TASK_START + _HW_DEF_FLASH_ADDR_TASK_LENGTH)



#endif

