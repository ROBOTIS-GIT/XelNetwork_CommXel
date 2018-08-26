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
  CHAR,
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

/* Send Callback */
void sendMsgStdBool(std_msgs::Bool* msg, void* arg);
void sendMsgStdChar(std_msgs::Char* msg, void* arg);
void sendMsgStdInt8(std_msgs::Int8* msg, void* arg);
void sendMsgStdInt16(std_msgs::Int16* msg, void* arg);
void sendMsgStdInt32(std_msgs::Int32* msg, void* arg);
void sendMsgStdInt64(std_msgs::Int64* msg, void* arg);
void sendMsgStdUint8(std_msgs::UInt8* msg, void* arg);
void sendMsgStdUint16(std_msgs::UInt16* msg, void* arg);
void sendMsgStdUint32(std_msgs::UInt32* msg, void* arg);
void sendMsgStdUint64(std_msgs::UInt64* msg, void* arg);
void sendMsgStdFloat32(std_msgs::Float32* msg, void* arg);
void sendMsgStdFloat64(std_msgs::Float64* msg, void* arg);

void sendMsgGeometryVector3(geometry_msgs::Vector3* msg, void* arg);
void sendMsgGeometryQauternion(geometry_msgs::Quaternion* msg, void* arg);
void sendMsgGeometryPoint(geometry_msgs::Point* msg, void* arg);
void sendMsgGeometryTwist(geometry_msgs::Twist* msg, void* arg);

void sendMsgSensorImu(sensor_msgs::Imu* msg, void* arg);

/* Receive Callback */
void recvMsgStdBool(std_msgs::Bool* msg, void* arg);
void recvMsgStdBool(std_msgs::Bool* msg, void* arg);
void recvMsgStdChar(std_msgs::Char* msg, void* arg);
void recvMsgStdInt8(std_msgs::Int8* msg, void* arg);
void recvMsgStdInt16(std_msgs::Int16* msg, void* arg);
void recvMsgStdInt32(std_msgs::Int32* msg, void* arg);
void recvMsgStdInt64(std_msgs::Int64* msg, void* arg);
void recvMsgStdUint8(std_msgs::UInt8* msg, void* arg);
void recvMsgStdUint16(std_msgs::UInt16* msg, void* arg);
void recvMsgStdUint32(std_msgs::UInt32* msg, void* arg);
void recvMsgStdUint64(std_msgs::UInt64* msg, void* arg);
void recvMsgStdFloat32(std_msgs::Float32* msg, void* arg);
void recvMsgStdFloat64(std_msgs::Float64* msg, void* arg);

void recvMsgGeometryTwist(geometry_msgs::Twist* msg, void* arg);

#endif /* XEL_NET_MSGS_HPP_ */

