/*
 * xel_net_msgs.cpp
 *
 *  Created on: Aug 22, 2018
 *      Author: kei
 */


#include "xel_net_msgs.hpp"
#include "xel/xel_struct.hpp"


using namespace XelNetwork;


/////////////////////////////////////////////////////////////
///// Message Callback
/////////////////////////////////////////////////////////////
void callbackMsgStdBool(std_msgs::Bool* msg, void* arg)
{
  XelInfo_t *p_xel = (XelInfo_t*)arg;
  Bool_t *p_data = (Bool_t*)p_xel->data;

  if(p_xel->header.data_direction == XelNetwork::SEND)
  {
    msg->data = p_data->data;
  }
  else
  {
    p_data->data = msg->data;
    printf("%d ", p_data->data);
    p_xel->status.flag_get_data = true;
  }
}

void callbackMsgStdChar(std_msgs::Char* msg, void* arg)
{
  XelInfo_t *p_xel = (XelInfo_t*)arg;
  Char_t *p_data = (Char_t*)p_xel->data;

  if(p_xel->header.data_direction == XelNetwork::SEND)
  {
    msg->data = p_data->data;
  }
  else
  {
    p_data->data = msg->data;
    p_xel->status.flag_get_data = true;
  }
}

void callbackMsgStdInt8(std_msgs::Int8* msg, void* arg)
{
  XelInfo_t *p_xel = (XelInfo_t*)arg;
  Int8_t *p_data = (Int8_t*)p_xel->data;

  if(p_xel->header.data_direction == XelNetwork::SEND)
  {
    msg->data = (uint8_t)p_data->data;
  }
  else
  {
    p_data->data = (int8_t)msg->data;
    p_xel->status.flag_get_data = true;
  }
}

void callbackMsgStdInt16(std_msgs::Int16* msg, void* arg)
{
  XelInfo_t *p_xel = (XelInfo_t*)arg;
  Int16_t *p_data = (Int16_t*)p_xel->data;

  if(p_xel->header.data_direction == XelNetwork::SEND)
  {
    msg->data = p_data->data;
  }
  else
  {
    p_data->data = msg->data;
    p_xel->status.flag_get_data = true;
  }
}

void callbackMsgStdInt32(std_msgs::Int32* msg, void* arg)
{
  XelInfo_t *p_xel = (XelInfo_t*)arg;
  Int32_t *p_data = (Int32_t*)p_xel->data;

  if(p_xel->header.data_direction == XelNetwork::SEND)
  {
    msg->data = p_data->data;
  }
  else
  {
    p_data->data = msg->data;
    p_xel->status.flag_get_data = true;
  }
}

void callbackMsgStdInt64(std_msgs::Int64* msg, void* arg)
{
  XelInfo_t *p_xel = (XelInfo_t*)arg;
  Int64_t *p_data = (Int64_t*)p_xel->data;

  if(p_xel->header.data_direction == XelNetwork::SEND)
  {
    msg->data = p_data->data;
  }
  else
  {
    p_data->data = msg->data;
    p_xel->status.flag_get_data = true;
  }
}

void callbackMsgStdUint8(std_msgs::UInt8* msg, void* arg)
{
  XelInfo_t *p_xel = (XelInfo_t*)arg;
  Uint8_t *p_data = (Uint8_t*)p_xel->data;

  if(p_xel->header.data_direction == XelNetwork::SEND)
  {
    msg->data = p_data->data;
  }
  else
  {
    p_data->data = msg->data;
    p_xel->status.flag_get_data = true;
  }
}

void callbackMsgStdUint16(std_msgs::UInt16* msg, void* arg)
{
  XelInfo_t *p_xel = (XelInfo_t*)arg;
  Uint16_t *p_data = (Uint16_t*)p_xel->data;

  if(p_xel->header.data_direction == XelNetwork::SEND)
  {
    msg->data = p_data->data;
  }
  else
  {
    p_data->data = msg->data;
    p_xel->status.flag_get_data = true;
  }
}

void callbackMsgStdUint32(std_msgs::UInt32* msg, void* arg)
{
  XelInfo_t *p_xel = (XelInfo_t*)arg;
  Uint32_t *p_data = (Uint32_t*)p_xel->data;

  printf("Uint32_t Callback : %d \r\n", p_xel->xel_id);

  if(p_xel->header.data_direction == XelNetwork::SEND)
  {
    msg->data = p_data->data;
  }
  else
  {
    p_data->data = msg->data;
    p_xel->status.flag_get_data = true;
  }
}

void callbackMsgStdUint64(std_msgs::UInt64* msg, void* arg)
{
  XelInfo_t *p_xel = (XelInfo_t*)arg;
  Uint64_t *p_data = (Uint64_t*)p_xel->data;

  if(p_xel->header.data_direction == XelNetwork::SEND)
  {
    msg->data = p_data->data;
  }
  else
  {
    p_data->data = msg->data;
    p_xel->status.flag_get_data = true;
  }
}

void callbackMsgStdFloat32(std_msgs::Float32* msg, void* arg)
{
  XelInfo_t *p_xel = (XelInfo_t*)arg;
  Float32_t *p_data = (Float32_t*)p_xel->data;

  if(p_xel->header.data_direction == XelNetwork::SEND)
  {
    msg->data = p_data->data;
  }
  else
  {
    p_data->data = msg->data;
    p_xel->status.flag_get_data = true;
  }
}

void callbackMsgStdFloat64(std_msgs::Float64* msg, void* arg)
{
  XelInfo_t *p_xel = (XelInfo_t*)arg;
  Float64_t *p_data = (Float64_t*)p_xel->data;

  if(p_xel->header.data_direction == XelNetwork::SEND)
  {
    msg->data = p_data->data;
  }
  else
  {
    p_data->data = msg->data;
    p_xel->status.flag_get_data = true;
  }
}


void callbackMsgGeometryVector3(geometry_msgs::Vector3* msg, void* arg)
{
  (void)(msg); (void)(arg);
}

void callbackMsgGeometryQauternion(geometry_msgs::Quaternion* msg, void* arg)
{
  (void)(msg); (void)(arg);
}

void callbackMsgGeometryPoint(geometry_msgs::Point* msg, void* arg)
{
  (void)(msg); (void)(arg);
}

void callbackMsgGeometryTwist(geometry_msgs::Twist* msg, void* arg)
{
  (void)(msg); (void)(arg);
}


void callbackMsgSensorImu(sensor_msgs::Imu* msg, void* arg)
{
  XelInfo_t *p_xel = (XelInfo_t*)arg;
  Imu_t *p_data = (Imu_t*)p_xel->data;

  if(p_xel->header.data_direction == XelNetwork::SEND)
  {
    msg->angular_velocity.x = p_data->ang_x;
    msg->angular_velocity.y = p_data->ang_y;
    msg->angular_velocity.z = p_data->ang_z;

    msg->linear_acceleration.x = p_data->acc_x;
    msg->linear_acceleration.y = p_data->acc_y;
    msg->linear_acceleration.z = p_data->acc_z;
  }
  else
  {
    //Not Support
  }
}
