/*
 * xel_net_msgs.hpp
 *
 *  Created on: Aug 23, 2018
 *      Author: kei
 */

#ifndef XEL_NET_MSGS_HPP_
#define XEL_NET_MSGS_HPP_

#include "ros2.hpp"

namespace XelNetwork
{

enum DataType
{
  BOOLEAN = 0,
  INT8,
  UINT8,
  INT16,
  UINT16,
  INT32,
  UINT32,
  INT64,
  UINT64,
  FLOAT32,
  FLOAT64,
  VECTOR3,
  QUATERNION,
  POINT,
  TWIST,
  IMU
};


} //namespace XelNetwork

//template <typename MsgT>
//void sendMsgCallback(MsgT* msg, void* arg);
//template <typename MsgT>
//void recvMsgCallback(MsgT* msg, void* arg);

void sendMsgStdBool(std_msgs::Bool* msg, void* arg);
void sendMsgSensorImu(sensor_msgs::Imu* msg, void* arg);

#endif /* XEL_NET_MSGS_HPP_ */
