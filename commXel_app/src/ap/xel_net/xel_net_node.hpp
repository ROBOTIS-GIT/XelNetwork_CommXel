/*
 * xel_net_node.hpp
 *
 *  Created on: Jul 17, 2018
 *      Author: kei
 */

#ifndef XEL_NET_NODE_HPP_
#define XEL_NET_NODE_HPP_

#include "ros2.hpp"
#include "xel_net_msgs.hpp"


namespace XelNetwork {

enum DataDirection
{
  SEND = 0,
  RECEIVE
};

enum XelStatus
{
  NOT_CONNECTTED = 0,
  NEW_CONNECTION,
  LOST_CONNECTION,
  RUNNING
};

struct XelInitData_t
{
  uint8_t              xel_id;
  XelNetwork::DataType data_type;
  uint32_t             data_get_interval_hz;
  char                 data_name[32];         //ROS2 topic name
  ros2::MessagePrefix  msg_type;              //ros2 message type (topic, service, action..)
  uint16_t             data_addr;
  uint8_t              data_lenght;
};

struct XelStatus_t
{
  XelStatus  previous;
  XelStatus  current;
  bool       flag_changed;
  bool       flag_get_data;
};

struct XelDDS_t
{
  ros2::CallbackFunc p_callback_func;
};

typedef struct XelInfo
{
  struct XelStatus_t   status;
  struct XelInitData_t init_data;
  uint8_t              data[128];
  struct XelDDS_t      dds;
} XelInfo_t;


class XelNetworkNode : public ros2::Node
{
public:
  XelNetworkNode()
  : Node()
  {
  }

  template <typename MsgT>
  bool createNewTopic(XelInfo_t* info)
  {
    bool ret = false;

    switch(info->init_data.msg_type)
    {
      case ros2::TOPICS_PUBLISH:
      {
        ros2::Publisher<MsgT>* p_pub = this->createPublisher<MsgT>((const char*)info->init_data.data_name);
        this->createWallFreq(info->init_data.data_get_interval_hz, (ros2::CallbackFunc)info->dds.p_callback_func, info->data, p_pub);
        ret = p_pub!=NULL ? true:false;
      }
        break;

      case ros2::TOPICS_SUBSCRIBE:
      {
        ros2::Subscriber<MsgT>* p_sub = this->createSubscriber<MsgT>((const char*)info->init_data.data_name,
            (ros2::CallbackFunc)info->dds.p_callback_func, info->data);
        ret = p_sub!=NULL ? true:false;
      }
        break;

      case ros2::SERVICE_REQUEST:
      {
      }
        break;

      case ros2::SERVICE_RESPONSE:
      {
      }
        break;

      case ros2::SERVICE:
      {
      }
        break;

      case ros2::PARAMETER:
      {
      }
        break;

      case ros2::ACTION:
      {
      }
        break;

      default:
        break;
    }

    return ret;
  }

private:

};


bool createNewTopicWithXel(XelNetworkNode* node, XelInfo_t* info);



} // namespace XelNetwork





#endif /* XEL_NET_NODE_HPP_ */
