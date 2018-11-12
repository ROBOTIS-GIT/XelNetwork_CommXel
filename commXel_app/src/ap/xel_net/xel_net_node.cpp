/*
 * xel_net_node.cpp
 *
 *  Created on: Aug 24, 2018
 *      Author: kei
 */


#include "xel_net_node.hpp"
#include "xel_net_msgs.hpp"
#include "xels/xels.h"

void subscribeTimeSync(builtin_interfaces::Time* msg, void* arg)
{
  (void)(arg);

  ros2::syncTimeFromRemote(msg);
}


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

    case MILLIS:
      info->dds.p_callback_func = (ros2::CallbackFunc)callbackMsgStdUint32;
      ret = node->createNewTopic<std_msgs::UInt32>(info);
      break;

    case LED:
      info->dds.p_callback_func = (ros2::CallbackFunc)callbackMsgStdBool;
      ret = node->createNewTopic<std_msgs::Bool>(info);
      break;

    case ANALOG0:
      info->dds.p_callback_func = (ros2::CallbackFunc)callbackMsgStdUint16;
      ret = node->createNewTopic<std_msgs::UInt16>(info);
      break;

    case ANALOG1:
      info->dds.p_callback_func = (ros2::CallbackFunc)callbackMsgStdUint16;
      ret = node->createNewTopic<std_msgs::UInt16>(info);
      break;

    case ANALOG2:
      info->dds.p_callback_func = (ros2::CallbackFunc)callbackMsgStdUint16;
      ret = node->createNewTopic<std_msgs::UInt16>(info);
      break;

    case ANALOG3:
      info->dds.p_callback_func = (ros2::CallbackFunc)callbackMsgStdUint16;
      ret = node->createNewTopic<std_msgs::UInt16>(info);
      break;

    case GPIO0:
      info->dds.p_callback_func = (ros2::CallbackFunc)callbackMsgStdBool;
      ret = node->createNewTopic<std_msgs::Bool>(info);
      break;

    case GPIO1:
      info->dds.p_callback_func = (ros2::CallbackFunc)callbackMsgStdBool;
      ret = node->createNewTopic<std_msgs::Bool>(info);
      break;

    case GPIO2:
      info->dds.p_callback_func = (ros2::CallbackFunc)callbackMsgStdBool;
      ret = node->createNewTopic<std_msgs::Bool>(info);
      break;

    case GPIO3:
      info->dds.p_callback_func = (ros2::CallbackFunc)callbackMsgStdBool;
      ret = node->createNewTopic<std_msgs::Bool>(info);
      break;

    case POWER:
      info->dds.p_callback_func = (ros2::CallbackFunc)callbackPowerXelPower;
      ret = node->createNewTopic<sensor_msgs::BatteryState>(info);
      break;

    case IMU:
      info->dds.p_callback_func = (ros2::CallbackFunc)callbackMsgSensorImu;
      ret = node->createNewTopic<sensor_msgs::Imu>(info);
      break;

    case JOYSTICK:
      info->dds.p_callback_func = (ros2::CallbackFunc)callbackMsgSensorJoy;
      ret = node->createNewTopic<sensor_msgs::Joy>(info);
      break;

    case I2CTEST:
      info->dds.p_callback_func = (ros2::CallbackFunc)callbackMsgTest;
      ret = node->createNewTopic<geometry_msgs::Point>(info);
      break;

    default:
      break;
  }

  return ret;
}
