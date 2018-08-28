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

struct XelHeader_t
{
  uint8_t                   id;
  XelNetwork::DataType      data_type;
  XelNetwork::DataDirection data_direction;
  uint32_t                  data_get_interval_hz;
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
  char                msg_name[32];         //ROS2 topic name
  ros2::MessagePrefix msg_type;             //ros2 message type (topic, service, action..)
  ros2::CallbackFunc  p_callback_func;
};

typedef struct XelInfo
{
  struct XelStatus_t status;
  struct XelHeader_t header;
  uint8_t            data[128];
  struct XelDDS_t    dds;
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

    switch(info->dds.msg_type)
    {
      case ros2::TOPICS_PUBLISH:
      {
        ros2::Publisher<MsgT>* p_pub = this->createPublisher<MsgT>((const char*)info->dds.msg_name);
        this->createWallFreq(info->header.data_get_interval_hz, (ros2::CallbackFunc)info->dds.p_callback_func, info->data, p_pub);
        ret = p_pub!=NULL ? true:false;
      }
        break;

      case ros2::TOPICS_SUBSCRIBE:
      {
        ros2::Subscriber<MsgT>* p_sub = this->createSubscriber<MsgT>((const char*)info->dds.msg_name,
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
