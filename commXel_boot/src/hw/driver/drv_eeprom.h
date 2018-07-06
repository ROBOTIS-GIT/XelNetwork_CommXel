/*
 *  drv_eeprom.h
 *
 *  Created on: 2017. 6. 9.
 *      Author: Baram
 */

#ifndef DRV_EEPROM_H_
#define DRV_EEPROM_H_


#ifdef __cplusplus
 extern "C" {
#endif


#include "hw_def.h"


bool drvEepromInit();

uint8_t  drvEepromReadByte(uint32_t addr);
bool     drvEepromWriteByte(uint32_t index, uint8_t data_in);
uint32_t drvEepromGetLength(void);
bool     drvEepromFormat(void);

#ifdef __cplusplus
}
#endif


#endif /* DRV_EXTI_H_ */

