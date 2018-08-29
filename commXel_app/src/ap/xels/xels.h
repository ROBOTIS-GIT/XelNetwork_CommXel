/*
 * xels.h
 *
 *  Created on: 2018. 8. 24.
 *      Author: HanCheol Cho
 */

#ifndef SRC_AP_XELS_XELS_H_
#define SRC_AP_XELS_XELS_H_


#include "hw.h"
#include "ap_def.h"
#include "xel/xel_struct.hpp"

#define _XELS_START_ID  1
#define _XELS_END_ID    10


typedef union
{
  uint8_t  u8Data[128];
  uint8_t  BOOLEAN;
  char     CHAR;
  int8_t   INT8;
  uint8_t  UINT8;
  int16_t  INT16;
  uint16_t UINT16;
  int32_t  INT32;
  uint32_t UINT32;
  int64_t  INT64;
  uint64_t UINT64;
  float    FLOAT32;
  double   FLOAT64;
  //VECTOR3,
  //QUATERNION,
  //POINT,
  //TWIST,
  //IMU
} xel_data_type_t;


void xelsInit(void);
bool xelsOpen(uint8_t ch, uint32_t baud);
uint32_t xelsPings(XelNetwork::XelInfo_t *p_xel_infos, uint32_t max_xels);
bool xelsPing(XelNetwork::XelInfo_t *p_xel_info);
bool xelsReadHeader(XelNetwork::XelInfo_t *p_xel_info);
bool xelsReadData(XelNetwork::XelInfo_t *p_xel_info);

#endif /* SRC_AP_XELS_XELS_H_ */
