/*
 * ros2.hpp
 *
 *  Created on: May 16, 2018
 *      Author: Kei
 */

#ifndef ROS2_HPP_
#define ROS2_HPP_

#include <string.h>
#include "publisher.hpp"
#include "subscriber.hpp"
#include "topic.hpp"
#include "builtin_interfaces/Time.hpp"


void onTopicCallback(mrSession* session, mrObjectId object_id, uint16_t request_id, mrStreamId stream_id, struct MicroBuffer* mb, void* args);


namespace ros2 {

extern char* client_communication_method;
extern char* server_ip;
extern uint16_t server_port;

class Node
{
  public:
    uint8_t err_code;

    Node()
    {
      err_code = 0, pub_cnt_ = 0, sub_cnt_ = 0, node_register_state_ = false;
      this->recreate();
    }

    virtual ~Node(){};

    void recreate()
    {
      err_code = 0;
      participant_.is_init = false;
      if(strcmp(client_communication_method, "Serial") == 0)
      {
        micrortps::setup(onTopicCallback, (void*) this);
      }
      else
      {
        micrortps::setup(server_ip, server_port, onTopicCallback, (void*) this);
      }

      node_register_state_ = micrortps::createParticipant(&this->participant_);

      uint8_t i;
      for(i = 0; i < pub_cnt_; i++)
      {
        if(pub_list_[i] != NULL)
        {
          pub_list_[i]->recreate();
        }
      }

      for(i = 0; i < sub_cnt_; i++)
      {
        if(sub_list_[i] != NULL)
        {
          sub_list_[i]->recreate();
        }
      }
    }

    template <
      typename MsgT>
    Publisher<MsgT>* createPublisher(const char* name)
    {
      bool ret = false;
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

      if(p_pub->is_registered_ == false)
      {
        err_code = 3;
        return NULL;
      }

      pub_list_[pub_cnt_++] = p_pub;
      err_code = 0;

      return p_pub;
    }

    template <
      typename MsgT>
    Subscriber<MsgT>* createSubscriber(const char* name, CallbackFunc callback)
    {
      bool ret = false;
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

      p_sub = new ros2::Subscriber<MsgT>(&this->participant_, name, callback);

      if(p_sub->is_registered_ == false)
      {
        err_code = 10 + 3;
        return NULL;
      }

      sub_list_[sub_cnt_++] = p_sub;
      p_sub->subscribe();
      err_code = 0;

      return p_sub;
    }

    void createWallTimer(uint32_t msec, CallbackFunc callback, PublisherHandle* pub)
    {
      if(pub == NULL)
      {
        return;
      }

      pub->setInterval(msec);
      pub->callback = callback;
    }

    void createWallFreq(uint32_t hz, CallbackFunc callback, PublisherHandle* pub)
    {
      uint32_t msec;
      if(hz > 1000)
      {
        hz = 1000;
      }
      msec = (uint32_t)(1000/hz);
      this->createWallTimer(msec, callback, pub);
    }

    void runPubCallback()
    {
      uint8_t i;
      ros2::PublisherHandle *p_pub;
      for(i = 0; i < pub_cnt_; i++)
      {
        p_pub = pub_list_[i];
        if(p_pub->is_registered_ && p_pub->isTimeToPublish())
        {
          p_pub->publish();
          break;
        }
      }
    }

    void runSubCallback(uint8_t topic_id, void* topic_msg)
    {
      uint8_t i;
      ros2::SubscriberHandle *p_sub;
      for(i = 0; i < sub_cnt_; i++)
      {
        p_sub = sub_list_[i];
        if(p_sub->is_registered_ && p_sub->topic_id_ == topic_id)
        {
          if(p_sub->callback != NULL)
          {
            p_sub->callback(topic_msg);
          }
          //p_sub->subscribe();
        }
      }
    }

    void runSession(uint32_t timeout_ms)
    {
      uint8_t read_data_status, i;
      ros2::SubscriberHandle *p_sub;
      for(i = 0; i < sub_cnt_; i++)
      {
        p_sub = sub_list_[i];
        if(p_sub->is_registered_)
        {
          mr_run_session_until_status(participant_.session, timeout_ms, &p_sub->request_id_, &read_data_status, 1);
        }
      }
    }

    PublisherHandle*  pub_list_[20];
    SubscriberHandle* sub_list_[20];


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
      ret = micrortps::registerTopic(&this->participant_, topic_profile, topic.id_);

      return ret;
    }
};


bool init(void);
bool init(const char* p_server_ip, uint16_t server_port);
void spin(Node *node);
int64_t getMillisTime(void);
builtin_interfaces::Time now();


} /* namespace ros2 */



#endif /* ROS2_HPP_ */
