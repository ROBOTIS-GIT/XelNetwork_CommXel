/*
 * xel_net_msgs.cpp
 *
 *  Created on: Aug 22, 2018
 *      Author: kei
 */

#include "xel_net_msgs.hpp"
#include "xel_net_struct.hpp"


using namespace XelNetwork;


/////////////////////////////////////////////////////////////
///// Send Message Callback
/////////////////////////////////////////////////////////////
void sendMsgStdBool(std_msgs::Bool* msg, void* arg)
{
  Bool_t *p_data = (Bool_t*)arg;
  msg->data = p_data->data;
}

void sendMsgStdChar(std_msgs::Char* msg, void* arg)
{
  Char_t *p_data = (Char_t*)arg;
  msg->data = p_data->data;
}

void sendMsgStdInt8(std_msgs::Int8* msg, void* arg)
{
  Int8_t *p_data = (Int8_t*)arg;
  msg->data = p_data->data;
}

void sendMsgStdInt16(std_msgs::Int16* msg, void* arg)
{
  Int16_t *p_data = (Int16_t*)arg;
  msg->data = p_data->data;
}

void sendMsgStdInt32(std_msgs::Int32* msg, void* arg)
{
  Int32_t *p_data = (Int32_t*)arg;
  msg->data = p_data->data;
}

void sendMsgStdInt64(std_msgs::Int64* msg, void* arg)
{
  Int64_t *p_data = (Int64_t*)arg;
  msg->data = p_data->data;
}

void sendMsgStdUint8(std_msgs::UInt8* msg, void* arg)
{
  Uint8_t *p_data = (Uint8_t*)arg;
  msg->data = p_data->data;
}

void sendMsgStdUint16(std_msgs::UInt16* msg, void* arg)
{
  Uint16_t *p_data = (Uint16_t*)arg;
  msg->data = p_data->data;
}

void sendMsgStdUint32(std_msgs::UInt32* msg, void* arg)
{
  Uint32_t *p_data = (Uint32_t*)arg;
  msg->data = p_data->data;
}

void sendMsgStdUint64(std_msgs::UInt64* msg, void* arg)
{
  Uint64_t *p_data = (Uint64_t*)arg;
  msg->data = p_data->data;
}

void sendMsgStdFloat32(std_msgs::Float32* msg, void* arg)
{
  Float32_t *p_data = (Float32_t*)arg;
  msg->data = p_data->data;
}

void sendMsgStdFloat64(std_msgs::Float64* msg, void* arg)
{
  Float64_t *p_data = (Float64_t*)arg;
  msg->data = p_data->data;
}


void sendMsgGeometryVector3(geometry_msgs::Vector3* msg, void* arg)
{

}

void sendMsgGeometryQauternion(geometry_msgs::Quaternion* msg, void* arg)
{

}

void sendMsgGeometryPoint(geometry_msgs::Point* msg, void* arg)
{

}

void sendMsgGeometryTwist(geometry_msgs::Twist* msg, void* arg)
{

}


void sendMsgSensorImu(sensor_msgs::Imu* msg, void* arg)
{
  Imu_t *p_data = (Imu_t*)arg;

  msg->angular_velocity.x = p_data->ang_x;
  msg->angular_velocity.y = p_data->ang_y;
  msg->angular_velocity.z = p_data->ang_z;

  msg->linear_acceleration.x = p_data->acc_x;
  msg->linear_acceleration.y = p_data->acc_y;
  msg->linear_acceleration.z = p_data->acc_z;
}




/////////////////////////////////////////////////////////////
///// Received Message Callback
/////////////////////////////////////////////////////////////
void recvMsgStdBool(std_msgs::Bool* msg, void* arg)
{
  Bool_t *p_data = (Bool_t*)arg;
  p_data->data = msg->data;
}

void recvMsgStdChar(std_msgs::Char* msg, void* arg)
{
  Char_t *p_data = (Char_t*)arg;
  p_data->data = msg->data;
}

void recvMsgStdInt8(std_msgs::Int8* msg, void* arg)
{
  Int8_t *p_data = (Int8_t*)arg;
  p_data->data = msg->data;
}

void recvMsgStdInt16(std_msgs::Int16* msg, void* arg)
{
  Int16_t *p_data = (Int16_t*)arg;
  p_data->data = msg->data;
}

void recvMsgStdInt32(std_msgs::Int32* msg, void* arg)
{
  Int32_t *p_data = (Int32_t*)arg;
  p_data->data = msg->data;
}

void recvMsgStdInt64(std_msgs::Int64* msg, void* arg)
{
  Int64_t *p_data = (Int64_t*)arg;
  p_data->data = msg->data;
}

void recvMsgStdUint8(std_msgs::UInt8* msg, void* arg)
{
  Uint8_t *p_data = (Uint8_t*)arg;
  p_data->data = msg->data;
}

void recvMsgStdUint16(std_msgs::UInt16* msg, void* arg)
{
  Uint16_t *p_data = (Uint16_t*)arg;
  p_data->data = msg->data;
}

void recvMsgStdUint32(std_msgs::UInt32* msg, void* arg)
{
  Uint32_t *p_data = (Uint32_t*)arg;
  p_data->data = msg->data;
}

void recvMsgStdUint64(std_msgs::UInt64* msg, void* arg)
{
  Uint64_t *p_data = (Uint64_t*)arg;
  p_data->data = msg->data;
}

void recvMsgStdFloat32(std_msgs::Float32* msg, void* arg)
{
  Float32_t *p_data = (Float32_t*)arg;
  p_data->data = msg->data;
}

void recvMsgStdFloat64(std_msgs::Float64* msg, void* arg)
{
  Float64_t *p_data = (Float64_t*)arg;
  p_data->data = msg->data;
}


void recvMsgGeometryTwist(geometry_msgs::Twist* msg, void* arg)
{
  ;
}

