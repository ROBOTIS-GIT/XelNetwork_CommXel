/*
 * xel_net_struct.hpp
 *
 *  Created on: Aug 23, 2018
 *      Author: kei
 */

#ifndef XEL_NET_STRUCT_HPP_
#define XEL_NET_STRUCT_HPP_

namespace XelNetwork
{

typedef struct Bool
{
  bool data;
}Bool_t;

typedef struct Char
{
  char data;
}Char_t;

typedef struct Int8
{
  int8_t data;
}Int8_t;

typedef struct Int16
{
  int16_t data;
}Int16_t;

typedef struct Int32
{
  int32_t data;
}Int32_t;

typedef struct Int64
{
  int64_t data;
}Int64_t;

typedef struct Uint8
{
  uint8_t data;
}Uint8_t;

typedef struct Uint16
{
  uint16_t data;
}Uint16_t;

typedef struct Uint32
{
  uint32_t data;
}Uint32_t;

typedef struct Uint64
{
  uint64_t data;
}Uint64_t;

typedef struct Float32
{
  float data;
}Float32_t;

typedef struct Float64
{
  double data;
}Float64_t;

typedef struct Imu
{
  double acc_x;
  double acc_y;
  double acc_z;
  double ang_x;
  double ang_y;
  double ang_z;
}Imu_t;

} //XelNetwork

#endif /* XEL_NET_STRUCT_HPP_ */