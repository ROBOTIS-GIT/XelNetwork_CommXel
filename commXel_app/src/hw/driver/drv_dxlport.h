/*
 * drv_dxlport.h
 *
 *  Created on: 2017. 2. 13.
 *      Author: baram
 */

#ifndef DRV_DXLPORT_H_
#define DRV_DXLPORT_H_



#ifdef __cplusplus
 extern "C" {
#endif


#include "hw_def.h"


#define DXLPORT_MAX_CH    _HW_DEF_DXLPORT_CH_MAX

bool drvDxlportInit(void);


bool drvDxlportOpen(uint8_t ch, uint32_t baud);
bool drvDxlportClose(uint8_t ch);
bool drvDxlportFlush(uint8_t ch);
void drvDxlportTxEnable(uint8_t ch);
void drvDxlportTxDisable(uint8_t ch);
void drvDxlportPowerEnable(uint8_t ch);
void drvDxlportPowerDisable(uint8_t ch);
bool drvDxlportGetPower(uint8_t ch);
void drvDxlportSetTxDoneISR(uint8_t ch, void (*p_txDoneISR)(void));
bool drvDxlportIsTxDone(uint8_t ch);

uint32_t drvDxlportGetBaud(uint8_t ch);

uint32_t drvDxlportAvailable(uint8_t ch);
uint8_t  drvDxlportRead(uint8_t ch);
uint32_t drvDxlportWrite(uint8_t ch, uint8_t *p_data, uint32_t length);




#ifdef __cplusplus
}
#endif

#endif /* DRV_DXLPORT_H_ */
