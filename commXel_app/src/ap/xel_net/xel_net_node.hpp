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
#include "xel/xel_struct.hpp"


namespace XelNetwork {


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
