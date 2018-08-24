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

typedef struct Int32
{
  int32_t data;
}Int32_t;

typedef struct Int64
{
  int64_t data;
}Int64_t;

typedef struct Float
{
  float data;
}Float_t;

typedef struct Double
{
  double data;
}Double_t;

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
