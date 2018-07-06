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
    {
      err_code = 0;
      participant_.is_init = false;
      node_register_state_ = micrortps::createParticipant(&this->participant_);
    }

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

      // Register Topic
      ret = this->registerTopic<MsgT>(name);

      if (ret == false)
      {
        err_code = 2;
        return NULL;
      }

      p_pub = new ros2::Publisher<MsgT>(&this->participant_, name);

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

      // Register Topic
      ret = this->registerTopic<MsgT>(name);

      if (ret == false)
      {
        err_code = 10 + 2;
        return NULL;
      }

      p_sub = new ros2::Subscriber<MsgT>(&this->participant_, name);

      if(p_sub->is_registered_ == false)
      {
        err_code = 10 + 3;
        return NULL;
      }

      err_code = 0;

      return p_sub;
    }


  private:
    bool node_register_state_;
    micrortps::Participant_t participant_;

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
void spin(void);


} /* namespace ros2 */



#endif /* ROS2_HPP_ */
