/*
 * xel_net_msgs.cpp
 *
 *  Created on: Aug 22, 2018
 *      Author: kei
 */


#include "xel_net_msgs.hpp"
#include "xels/xels.h"

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





void callbackPublishDXL(sensor_msgs::JointState* msg, void* arg)
{
//  XelInfo_t *p_xels = (XelInfo_t*)arg;
//  XelInfo_t *p_xel;
//  uint8_t i, dxl_cnt = 0;
//  int id_num = 0;
//
//  memset(msg, 0, sizeof(sensor_msgs::JointState));
//
//  for(i = 0; i < CONNECTED_XEL_MAX; i++)
//  {
//    p_xel = p_xels+i;
//    if (p_xel->model_id != XELS_SENSORXEL_MODEL_ID
//        && p_xel->model_id != XELS_POWERXEL_MODEL_ID
//        && p_xel->model_id != XELS_COMMXEL_MODEL_ID)
//    {
//      itoa(id_num, msg->name[dxl_cnt], 10);
//      p_xel->xel_id = (uint8_t)id_num;
//      memcpy(&msg->position[dxl_cnt], p_xel->data, sizeof(msg->position[dxl_cnt]));
//      dxl_cnt++;
//      if(dxl_cnt == 20)
//      {
//        break;
//      }
//    }
//  }
//
//  strcpy(msg->header.frame_id, "DynamiXels");
//  msg->header.stamp = ros2::now();
//  msg->name_size = dxl_cnt;
//  msg->position_size = dxl_cnt;
}

void callbackSubscribeDXL(sensor_msgs::JointState* msg, void* arg)
{
//  XelInfo_t *p_xels = (XelInfo_t*)arg;
//  XelInfo_t *p_xel;
//  uint8_t i, j;
//
//  for(i = 0; i < 20; i++)
//  {
//    if(msg->name[i] != 0)
//    {
//      // Search correspond ID
//      for(j = 0; j < CONNECTED_XEL_MAX; j++)
//      {
//        p_xel = p_xels+j;
//        if (p_xel->model_id != XELS_SENSORXEL_MODEL_ID
//            && p_xel->model_id != XELS_POWERXEL_MODEL_ID
//            && p_xel->model_id != XELS_COMMXEL_MODEL_ID)
//        {
//          if (p_xel->xel_id == atoi(msg->name[i]))
//          {
//            memcpy(p_xel->data, &msg->position[i], sizeof(msg->position[i]));
//            p_xel->status.flag_get_data = true;
//          }
//        }
//      }
//    }
//  }
}
