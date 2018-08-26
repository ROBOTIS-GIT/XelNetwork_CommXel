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

typedef struct XelInfo
{
  uint8_t              status;
  bool                 is_connected;
  uint8_t              xel_id;
  XelNetwork::DataType data_type;
  char                 data_name[32];
  ros2::MessagePrefix  msg_type;
  uint32_t             pub_interval_hz;
  uint8_t              data[128];
  ros2::CallbackFunc   p_callback_func;
  bool                 is_registered;
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

    switch(info->msg_type)
    {
      case ros2::TOPICS_PUBLISH:
      {
        ros2::Publisher<MsgT>* p_pub = this->createPublisher<MsgT>((const char*)info->data_name);
        this->createWallFreq(info->pub_interval_hz, (ros2::CallbackFunc)info->p_callback_func, info->data, p_pub);
        ret = p_pub!=NULL ? true:false;
      }
        break;

      case ros2::TOPICS_SUBSCRIBE:
      {
        ros2::Subscriber<MsgT>* p_sub = this->createSubscriber<MsgT>((const char*)info->data_name,
            (ros2::CallbackFunc)info->p_callback_func, info->data);
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
