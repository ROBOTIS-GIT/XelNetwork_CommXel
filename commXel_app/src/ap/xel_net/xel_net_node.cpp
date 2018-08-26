/*
 * xel_net_node.cpp
 *
 *  Created on: Aug 24, 2018
 *      Author: kei
 */


#include "xel_net_node.hpp"
#include "xel_net_msgs.hpp"


bool XelNetwork::createNewTopicWithXel(XelNetwork::XelNetworkNode* node, XelNetwork::XelInfo_t* info)
{
  bool ret = false;

  switch(info->data_type)
  {
    case BOOLEAN:
      //ret = node->createNewTopic<std_msgs::Bool>(info);
      break;

    case INT8:
      //ret = node->createNewTopic<std_msgs::Bool>(info);
      break;

    case UINT8:
      //ret = node->createNewTopic<std_msgs::Bool>(info);
      break;

    case INT16:
      //ret = node->createNewTopic<std_msgs::Bool>(info);
      break;

    case UINT16:
      //ret = node->createNewTopic<std_msgs::Bool>(info);
      break;

    case INT32:
      //ret = node->createNewTopic<std_msgs::Int32>(info);
      break;

    case UINT32:
      //ret = node->createNewTopic<std_msgs::Bool>(info);
      break;

    case INT64:
      //ret = node->createNewTopic<std_msgs::Int64>(info);
      break;

    case UINT64:
      //ret = node->createNewTopic<std_msgs::Bool>(info);
      break;

    case FLOAT32:
      //ret = node->createNewTopic<std_msgs::Bool>(info);
      break;

    case FLOAT64:
      //ret = node->createNewTopic<std_msgs::Bool>(info);
      break;

    case VECTOR3:
      //ret = node->createNewTopic<geometry_msgs::Vector3>(info);
      break;

    case QUATERNION:
      //ret = node->createNewTopic<geometry_msgs::Quaternion>(info);
      break;

    case POINT:
      //ret = node->createNewTopic<geometry_msgs::Point>(info);
      break;

    case TWIST:
      //ret = node->createNewTopic<geometry_msgs::Twist>(info);
      break;

    case IMU:
      info->p_callback_func = (ros2::CallbackFunc)sendMsgSensorImu;
      ret = node->createNewTopic<sensor_msgs::Imu>(info);
      break;

    default:
      break;
  }

  info->is_registered = ret;
  return ret;
}
