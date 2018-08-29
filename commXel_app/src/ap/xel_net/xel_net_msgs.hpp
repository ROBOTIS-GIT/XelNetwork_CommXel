/*
 * xel_net_msgs.hpp
 *
 *  Created on: Aug 23, 2018
 *      Author: kei
 */

#ifndef XEL_NET_MSGS_HPP_
#define XEL_NET_MSGS_HPP_

#include "ros2.hpp"


//template <typename MsgT>
//void callbackMsgCallback(MsgT* msg, void* arg);
//template <typename MsgT>
//void recvMsgCallback(MsgT* msg, void* arg);

/* Callback */
void callbackMsgStdBool(std_msgs::Bool* msg, void* arg);
void callbackMsgStdChar(std_msgs::Char* msg, void* arg);
void callbackMsgStdInt8(std_msgs::Int8* msg, void* arg);
void callbackMsgStdInt16(std_msgs::Int16* msg, void* arg);
void callbackMsgStdInt32(std_msgs::Int32* msg, void* arg);
void callbackMsgStdInt64(std_msgs::Int64* msg, void* arg);
void callbackMsgStdUint8(std_msgs::UInt8* msg, void* arg);
void callbackMsgStdUint16(std_msgs::UInt16* msg, void* arg);
void callbackMsgStdUint32(std_msgs::UInt32* msg, void* arg);
void callbackMsgStdUint64(std_msgs::UInt64* msg, void* arg);
void callbackMsgStdFloat32(std_msgs::Float32* msg, void* arg);
void callbackMsgStdFloat64(std_msgs::Float64* msg, void* arg);

void callbackMsgGeometryVector3(geometry_msgs::Vector3* msg, void* arg);
void callbackMsgGeometryQauternion(geometry_msgs::Quaternion* msg, void* arg);
void callbackMsgGeometryPoint(geometry_msgs::Point* msg, void* arg);
void callbackMsgGeometryTwist(geometry_msgs::Twist* msg, void* arg);

void callbackMsgSensorImu(sensor_msgs::Imu* msg, void* arg);


#endif /* XEL_NET_MSGS_HPP_ */

