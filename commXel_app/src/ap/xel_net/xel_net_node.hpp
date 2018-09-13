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

void subscribeTimeSync(builtin_interfaces::Time* msg, void* arg);

namespace XelNetwork {

const uint32_t DXL_STATUS_PUB_HZ  = 10;

class XelNetworkNode : public ros2::Node
{
public:
  XelNetworkNode()
  : Node(), is_created_dxl_topic(false)
  {
    this->createSubscriber<builtin_interfaces::Time>("time_sync", (ros2::CallbackFunc)subscribeTimeSync, NULL);
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
        ros2::Publisher<MsgT>* p_pub = this->createPublisher<MsgT>((const char*)info->header.data_name);
        this->createWallFreq(info->header.data_get_interval_hz, (ros2::CallbackFunc)info->dds.p_callback_func, info, p_pub);
        if(p_pub != NULL)
        {
          ret = true;
        }

        ros2::Subscriber<MsgT>* p_sub = this->createSubscriber<MsgT>((const char*)info->header.data_name,
            (ros2::CallbackFunc)info->dds.p_callback_func, info);
        if(p_sub != NULL)
        {
          info->dds.entity_id = p_sub->reader_id_;
          ret = true;
        }
      }
        break;

      default:
        break;
    }

    return ret;
  }

  void createDxlTopic(XelInfo_t* info)
  {
    ros2::Publisher<sensor_msgs::JointState>* p_pub = this->createPublisher<sensor_msgs::JointState>((const char*)"joint_states");
    this->createWallFreq(DXL_STATUS_PUB_HZ, (ros2::CallbackFunc)callbackPublishDXL, info, p_pub);
    is_created_dxl_topic = (p_pub!=NULL?true:false);

    ros2::Subscriber<sensor_msgs::JointState>* p_sub = this->createSubscriber<sensor_msgs::JointState>((const char*)"cmd_dynamixel", (ros2::CallbackFunc)callbackSubscribeDXL, info);
    is_created_dxl_topic = (p_sub!=NULL?true:false);
  }

  bool is_created_dxl_topic;

private:
};


bool createNewTopicWithXel(XelNetworkNode* node, XelInfo_t* info);



} // namespace XelNetwork





#endif /* XEL_NET_NODE_HPP_ */
