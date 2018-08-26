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
  bool direction;

  if(node == NULL || info == NULL)
  {
    return false;
  }

  if(info->msg_type == ros2::TOPICS_SUBSCRIBE
            || info->msg_type == ros2::SERVICE_RESPONSE
            || info->msg_type == ros2::PARAMETER)
  {
    direction = XelNetwork::RECEIVE;
  }
  else
  {
    direction = XelNetwork::SEND;
  }

  switch(info->data_type)
  {
    case BOOLEAN:
      if(direction == XelNetwork::SEND){
        info->p_callback_func = (ros2::CallbackFunc)sendMsgStdBool;
      }else{
        info->p_callback_func = (ros2::CallbackFunc)recvMsgStdBool;
      }
      ret = node->createNewTopic<std_msgs::Bool>(info);
      break;

    case CHAR:
      if(direction == XelNetwork::SEND){
        info->p_callback_func = (ros2::CallbackFunc)sendMsgStdChar;
      }else{
        info->p_callback_func = (ros2::CallbackFunc)recvMsgStdChar;
      }
      ret = node->createNewTopic<std_msgs::Char>(info);
      break;

    case INT8:
      if(direction == XelNetwork::SEND){
        info->p_callback_func = (ros2::CallbackFunc)sendMsgStdInt8;
      }else{
        info->p_callback_func = (ros2::CallbackFunc)recvMsgStdInt8;
      }
      ret = node->createNewTopic<std_msgs::Int8>(info);
      break;

    case INT16:
      if(direction == XelNetwork::SEND){
        info->p_callback_func = (ros2::CallbackFunc)sendMsgStdInt16;
      }else{
        info->p_callback_func = (ros2::CallbackFunc)recvMsgStdInt16;
      }
      ret = node->createNewTopic<std_msgs::Int16>(info);
      break;

    case INT32:
      if(direction == XelNetwork::SEND){
        info->p_callback_func = (ros2::CallbackFunc)sendMsgStdInt32;
      }else{
        info->p_callback_func = (ros2::CallbackFunc)recvMsgStdInt32;
      }
      ret = node->createNewTopic<std_msgs::Int32>(info);
      break;

    case INT64:
      if(direction == XelNetwork::SEND){
        info->p_callback_func = (ros2::CallbackFunc)sendMsgStdInt64;
      }else{
        info->p_callback_func = (ros2::CallbackFunc)recvMsgStdInt64;
      }
      ret = node->createNewTopic<std_msgs::Int64>(info);
      break;

    case UINT8:
      if(direction == XelNetwork::SEND){
        info->p_callback_func = (ros2::CallbackFunc)sendMsgStdUint8;
      }else{
        info->p_callback_func = (ros2::CallbackFunc)recvMsgStdUint8;
      }
      ret = node->createNewTopic<std_msgs::UInt8>(info);
      break;

    case UINT16:
      if(direction == XelNetwork::SEND){
        info->p_callback_func = (ros2::CallbackFunc)sendMsgStdUint16;
      }else{
        info->p_callback_func = (ros2::CallbackFunc)recvMsgStdUint16;
      }
      ret = node->createNewTopic<std_msgs::UInt16>(info);
      break;

    case UINT32:
      if(direction == XelNetwork::SEND){
        info->p_callback_func = (ros2::CallbackFunc)sendMsgStdUint32;
      }else{
        info->p_callback_func = (ros2::CallbackFunc)recvMsgStdUint32;
      }
      ret = node->createNewTopic<std_msgs::UInt32>(info);
      break;

    case UINT64:
      if(direction == XelNetwork::SEND){
        info->p_callback_func = (ros2::CallbackFunc)sendMsgStdUint64;
      }else{
        info->p_callback_func = (ros2::CallbackFunc)recvMsgStdUint64;
      }
      ret = node->createNewTopic<std_msgs::UInt64>(info);
      break;

    case FLOAT32:
      if(direction == XelNetwork::SEND){
        info->p_callback_func = (ros2::CallbackFunc)sendMsgStdFloat32;
      }else{
        info->p_callback_func = (ros2::CallbackFunc)recvMsgStdFloat32;
      }
      ret = node->createNewTopic<std_msgs::Float32>(info);
      break;

    case FLOAT64:
      if(direction == XelNetwork::SEND){
        info->p_callback_func = (ros2::CallbackFunc)sendMsgStdFloat64;
      }else{
        info->p_callback_func = (ros2::CallbackFunc)recvMsgStdFloat64;
      }

      ret = node->createNewTopic<std_msgs::Float64>(info);
      break;

    case VECTOR3:
      if(direction == XelNetwork::SEND){
        info->p_callback_func = (ros2::CallbackFunc)sendMsgGeometryVector3;
      }else{
        info->p_callback_func = (ros2::CallbackFunc)recvMsgStdChar;
      }
      ret = node->createNewTopic<geometry_msgs::Vector3>(info);
      break;

    case QUATERNION:
      if(direction == XelNetwork::SEND){
        info->p_callback_func = (ros2::CallbackFunc)sendMsgGeometryQauternion;
      }else{
        info->p_callback_func = NULL;
      }
      ret = node->createNewTopic<geometry_msgs::Quaternion>(info);
      break;

    case POINT:
      if(direction == XelNetwork::SEND){
        info->p_callback_func = (ros2::CallbackFunc)sendMsgGeometryPoint;
      }else{
        info->p_callback_func = NULL;
      }
      ret = node->createNewTopic<geometry_msgs::Point>(info);
      break;

    case TWIST:
      if(direction == XelNetwork::SEND){
        info->p_callback_func = (ros2::CallbackFunc)sendMsgGeometryTwist;
      }else{
        info->p_callback_func = (ros2::CallbackFunc)recvMsgGeometryTwist;
      }
      ret = node->createNewTopic<geometry_msgs::Twist>(info);
      break;

    case IMU:
      if(direction == XelNetwork::SEND){
        info->p_callback_func = (ros2::CallbackFunc)sendMsgSensorImu;
      }else{
        info->p_callback_func = NULL;
      }
      ret = node->createNewTopic<sensor_msgs::Imu>(info);
      break;

    default:
      break;
  }

  info->is_registered = ret;
  return ret;
}
