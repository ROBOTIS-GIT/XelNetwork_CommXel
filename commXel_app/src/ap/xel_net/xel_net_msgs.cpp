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


void callbackMsgSensorImu(sensor_msgs::Imu* msg, void* arg)
{
  XelInfo_t *p_xel = (XelInfo_t*)arg;
  Imu_t *p_data = (Imu_t*)p_xel->data;

  if(p_xel->header.data_direction == XelNetwork::SEND)
  {
    strcpy(msg->header.frame_id, "world");

    msg->angular_velocity.x = p_data->gyro_x;
    msg->angular_velocity.y = p_data->gyro_y;
    msg->angular_velocity.z = p_data->gyro_z;

    msg->linear_acceleration.x = p_data->acc_x;
    msg->linear_acceleration.y = p_data->acc_y;
    msg->linear_acceleration.z = p_data->acc_z;
  }
  else
  {
    //Not Support
  }
}

void callbackMsgSensorJoy(sensor_msgs::Joy* msg, void* arg)
{
  XelInfo_t *p_xel = (XelInfo_t*)arg;
  JoyStick_t *p_data = (JoyStick_t*)p_xel->data;

  if(p_xel->header.data_direction == XelNetwork::SEND)
  {
    strcpy(msg->header.frame_id, "world");
    msg->header.stamp = ros2::now();
    msg->axes[0] = (float)p_data->axis_x;
    msg->axes[1] = (float)p_data->axis_y;
    msg->axes_size = 2;
    msg->buttons[0] = (int32_t)p_data->button;
    msg->buttons_size = 1;
  }
  else
  {
    p_data->axis_x = (uint32_t)msg->axes[0];
    p_data->axis_y = (uint32_t)msg->axes[1];
    p_data->button = (uint32_t)msg->buttons[0];
  }
}





/* For Dynamixel */
int32_t convertRadian2Value(float radian)
{
  int32_t value = 0;

  if (radian > 0)
  {
    value = (radian * (4096 - 2048) / 3.14) + 2048;
  }
  else if (radian < 0)
  {
    value = (radian * (0 - 2048) / -3.14) + 2048;
  }
  else
  {
    value = 2048;
  }

  return value;
}

float convertValue2Radian(int32_t value)
{
  float radian = 0.0;

  if (value > 2048)
  {
    radian = (float)(value - 2048) * 3.14 / (float)(4096 - 2048);
  }
  else if (value < 2048)
  {
    radian = (float)(value - 2048) * -3.14 / (float)(0 - 2048);
  }

  return radian;
}


extern osSemaphoreId dxl_semaphore;

void callbackPublishDXL(sensor_msgs::JointState* msg, void* arg)
{
  //TODO:
  XelInfo_t *p_xels = (XelInfo_t*)arg;
  XelInfo_t *p_xel;
  uint8_t i, dxl_cnt = 0;
  DXLJoint_t joint_state;

  for(i = 0; i < CONNECTED_XEL_MAX; i++)
  {
    p_xel = &p_xels[i];
    if (p_xel->model_id != XELS_SENSORXEL_MODEL_ID
        && p_xel->model_id != XELS_POWERXEL_MODEL_ID
        && p_xel->model_id != XELS_COMMXEL_MODEL_ID)
    {
      if(p_xel->status.current == XelNetwork::RUNNING)
      {
        itoa((int)p_xel->xel_id, msg->name[dxl_cnt], 10);
        memcpy(&joint_state, p_xel->data, sizeof(DXLJoint_t));
        msg->position[dxl_cnt] = (double)convertValue2Radian(joint_state.position);
        msg->velocity[dxl_cnt] = (double)joint_state.velocity;
        msg->effort[dxl_cnt] = (double)joint_state.current;
        dxl_cnt++;

        if(dxl_cnt == 10)
        {
          break;
        }
      }
    }
  }

  strcpy(msg->header.frame_id, "world");
  msg->header.stamp = ros2::now();
  msg->name_size = dxl_cnt;
  msg->position_size = dxl_cnt;
}

void callbackSubscribeDXL(sensor_msgs::JointState* msg, void* arg)
{
  //TODO:
  XelInfo_t *p_xels = (XelInfo_t*)arg;
  XelInfo_t *p_xel;
  uint8_t i, j;
  DXLJoint_t data;

  for(i = 0; i < 10; i++)
  {
    if(msg->name[i] != 0)
    {
      // Search correspond ID
      for(j = 0; j < CONNECTED_XEL_MAX; j++)
      {
        p_xel = &p_xels[j];
        if (p_xel->model_id != XELS_SENSORXEL_MODEL_ID
            && p_xel->model_id != XELS_POWERXEL_MODEL_ID
            && p_xel->model_id != XELS_COMMXEL_MODEL_ID)
        {
          if (p_xel->xel_id == atoi(msg->name[i]) && p_xel->status.current == XelNetwork::RUNNING)
          {
            data.position = (int32_t)convertRadian2Value(msg->position[i]);
            data.velocity = (int32_t)msg->velocity[i];
            data.current = (int16_t)msg->effort[i];
            if(osSemaphoreWait(dxl_semaphore, 0) == osOK)
            {
              xelWriteDXLJointState(p_xel, &data);
              osSemaphoreRelease(dxl_semaphore);
            }
            //TODO:
            //p_xel->status.flag_get_data = true;
          }
        }
      }
    }
  }
}


void callbackPowerXelPower(sensor_msgs::BatteryState* msg, void* arg)
{
  XelInfo_t *p_xel = (XelInfo_t*)arg;
  Power_t *p_data = (Power_t*)p_xel->data;
  static float voltage;
  uint32_t voltage_int, current_int;

  if(p_xel->header.data_direction == XelNetwork::SEND)
  {
    strcpy(msg->header.frame_id, "world");
    msg->header.stamp = ros2::now();

    memcpy(&voltage_int, &p_data[0], sizeof(voltage_int));
    memcpy(&current_int, &p_data[4], sizeof(current_int));

    voltage = (float)(voltage_int/100) + ((float)(voltage_int%100)/100);

    msg->voltage = voltage;
    msg->current = (float)current_int;
    msg->cell_voltage_size = 1;
  }
  else
  {
    //Not Support
  }
}
