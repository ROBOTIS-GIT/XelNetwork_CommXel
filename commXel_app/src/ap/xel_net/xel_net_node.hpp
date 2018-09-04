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

    switch(info->header.data_direction)
    {
      case XelNetwork::SEND:
      {
        ros2::Publisher<MsgT>* p_pub = this->createPublisher<MsgT>((const char*)info->header.data_name);
        this->createWallFreq(info->header.data_get_interval_hz, (ros2::CallbackFunc)info->dds.p_callback_func, info, p_pub);
        if(p_pub != NULL)
        {
          info->dds.entity_id = p_pub->writer_id_;
          ret = true;
        }
      }
        break;

      case XelNetwork::RECEIVE:
      {
        ros2::Subscriber<MsgT>* p_sub = this->createSubscriber<MsgT>((const char*)info->header.data_name,
            (ros2::CallbackFunc)info->dds.p_callback_func, info);
        if(p_sub != NULL)
        {
          info->dds.entity_id = p_sub->reader_id_;
          ret = true;
        }
      }
        break;

      case XelNetwork::SEND_RECV:
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
