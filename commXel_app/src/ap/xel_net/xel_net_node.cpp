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

  if(node == NULL || info == NULL)
  {
    return false;
  }

  switch(info->header.data_type)
  {
    case BOOLEAN:
      info->dds.p_callback_func = (ros2::CallbackFunc)callbackMsgStdBool;
      ret = node->createNewTopic<std_msgs::Bool>(info);
      break;

    case CHAR:
      info->dds.p_callback_func = (ros2::CallbackFunc)callbackMsgStdChar;
      ret = node->createNewTopic<std_msgs::Char>(info);
      break;

    case INT8:
      info->dds.p_callback_func = (ros2::CallbackFunc)callbackMsgStdInt8;
      ret = node->createNewTopic<std_msgs::Int8>(info);
      break;

    case INT16:
      info->dds.p_callback_func = (ros2::CallbackFunc)callbackMsgStdInt16;
      ret = node->createNewTopic<std_msgs::Int16>(info);
      break;

    case INT32:
      info->dds.p_callback_func = (ros2::CallbackFunc)callbackMsgStdInt32;
      ret = node->createNewTopic<std_msgs::Int32>(info);
      break;

    case INT64:
      info->dds.p_callback_func = (ros2::CallbackFunc)callbackMsgStdInt64;
      ret = node->createNewTopic<std_msgs::Int64>(info);
      break;

    case UINT8:
      info->dds.p_callback_func = (ros2::CallbackFunc)callbackMsgStdUint8;
      ret = node->createNewTopic<std_msgs::UInt8>(info);
      break;

    case UINT16:
      info->dds.p_callback_func = (ros2::CallbackFunc)callbackMsgStdUint16;
      ret = node->createNewTopic<std_msgs::UInt16>(info);
      break;

    case UINT32:
      info->dds.p_callback_func = (ros2::CallbackFunc)callbackMsgStdUint32;
      ret = node->createNewTopic<std_msgs::UInt32>(info);
      break;

    case UINT64:
      info->dds.p_callback_func = (ros2::CallbackFunc)callbackMsgStdUint64;
      ret = node->createNewTopic<std_msgs::UInt64>(info);
      break;

    case FLOAT32:
      info->dds.p_callback_func = (ros2::CallbackFunc)callbackMsgStdFloat32;
      ret = node->createNewTopic<std_msgs::Float32>(info);
      break;

    case FLOAT64:
      info->dds.p_callback_func = (ros2::CallbackFunc)callbackMsgStdFloat64;
      ret = node->createNewTopic<std_msgs::Float64>(info);
      break;

    case VECTOR3:
      info->dds.p_callback_func = (ros2::CallbackFunc)callbackMsgGeometryVector3;
      ret = node->createNewTopic<geometry_msgs::Vector3>(info);
      break;

    case QUATERNION:
      info->dds.p_callback_func = (ros2::CallbackFunc)callbackMsgGeometryQauternion;
      ret = node->createNewTopic<geometry_msgs::Quaternion>(info);
      break;

    case POINT:
      info->dds.p_callback_func = (ros2::CallbackFunc)callbackMsgGeometryPoint;
      ret = node->createNewTopic<geometry_msgs::Point>(info);
      break;

    case TWIST:
      info->dds.p_callback_func = (ros2::CallbackFunc)callbackMsgGeometryTwist;
      ret = node->createNewTopic<geometry_msgs::Twist>(info);
      break;

    case IMU:
      info->dds.p_callback_func = (ros2::CallbackFunc)callbackMsgSensorImu;
      ret = node->createNewTopic<sensor_msgs::Imu>(info);
      break;

    default:
      break;
  }

  return ret;
}
