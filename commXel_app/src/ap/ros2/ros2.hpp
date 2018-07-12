/*
 * ros2.hpp
 *
 *  Created on: May 16, 2018
 *      Author: Kei
 */

#ifndef ROS2_HPP_
#define ROS2_HPP_

#include "publisher.hpp"
#include "subscriber.hpp"



namespace ros2 {

class Node
{
  public:
    uint8_t err_code;

    Node()
      : pub_cnt_(0), sub_cnt_(0)
    {
      err_code = 0;
      participant_.is_init = false;
      node_register_state_ = micrortps::createParticipant(&this->participant_);
    }

    virtual ~Node(){};

    template <
      typename MsgT>
    Publisher<MsgT>* createPublisher(const char* name)
    {
      bool ret;
      ros2::Publisher<MsgT> *p_pub = NULL;

      if(this->node_register_state_ == false)
      {
        err_code = 1;
        return NULL;
      }

      if(pub_cnt_ >= 20)
      {
        return NULL;
      }

      // Register Topic
      ret = this->registerTopic<MsgT>(name);

      if (ret == false)
      {
        err_code = 2;
        return NULL;
      }

      p_pub = new ros2::Publisher<MsgT>(&this->participant_, name);
      pub_list_[pub_cnt_++] = (NodeHandle*) p_pub;

      if(p_pub->is_registered_ == false)
      {
        err_code = 3;
        return NULL;
      }

      err_code = 0;

      return p_pub;
    }


    template <
      typename MsgT>
    Subscriber<MsgT>* createSubscriber(const char* name)
    {
      bool ret;
      ros2::Subscriber<MsgT> *p_sub = NULL;
      
      if(this->node_register_state_ == false)
      {
        err_code = 10 + 1;
        return NULL;
      }

      if(sub_cnt_ >= 20)
      {
        return NULL;
      }

      // Register Topic
      ret = this->registerTopic<MsgT>(name);

      if (ret == false)
      {
        err_code = 10 + 2;
        return NULL;
      }

      p_sub = new ros2::Subscriber<MsgT>(&this->participant_, name);
      sub_list_[sub_cnt_++] = (NodeHandle*) p_sub;

      if(p_sub->is_registered_ == false)
      {
        err_code = 10 + 3;
        return NULL;
      }

      err_code = 0;

      return p_sub;
    }

    virtual void callback(void) = 0;

    NodeHandle* pub_list_[20];
    NodeHandle* sub_list_[20];

  private:
    bool node_register_state_;
    micrortps::Participant_t participant_;
    uint8_t pub_cnt_;
    uint8_t sub_cnt_;

    template <
      typename MsgT>
    bool registerTopic(const char* name)
    {
      bool ret;
      MsgT topic;

      if(this->node_register_state_ == false)
      {
        return false;
      }

      char topic_profile[256] = {0, };
      sprintf(topic_profile, DEFAULT_TOPIC_XML, name, topic.type_);
      ret = micrortps::registerTopic(&this->participant_, topic_profile);

      return ret;
    }
};


bool init(OnTopic callback);
bool init(const uint8_t* p_server_ip, uint16_t server_port, OnTopic callback);
void spin(Node *node);


} /* namespace ros2 */



#endif /* ROS2_HPP_ */
