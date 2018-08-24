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

void sendMsgSensorImu(sensor_msgs::Imu* msg, void* arg)
{
  Imu_t *p_data = (Imu_t*)arg;

  strcpy(msg->header.frame_id, "test IMU");

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
void recvMessageCallback(std_msgs::Bool* msg, void* arg)
{

}

