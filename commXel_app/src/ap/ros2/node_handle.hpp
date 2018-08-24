/*
 * node_handle.hpp
 *
 *  Created on: May 16, 2018
 *      Author: Kei
 */

#ifndef ROS2_NODE_HANDLE_HPP_
#define ROS2_NODE_HANDLE_HPP_

#include <stdio.h>
#include "micrortps.hpp"
#include "hw.h"


namespace ros2 {

typedef void(*CallbackFunc)(void* msg, void* arg);

class PublisherHandle
{
 
public:
  PublisherHandle()
    : is_registered_(false)
  {
    callback = NULL;
    callback_arg = NULL;
    pub_msg_cnt_ = 0;
    callback_interval_ms_ = 0;
    last_call_time_ms_ = 0;
  }
  virtual ~PublisherHandle(){}

  CallbackFunc callback;
  void*        callback_arg;
  virtual void recreate(void) = 0;
  virtual void publish(void) = 0;

  void setInterval(uint32_t msec)
  {
    this->callback_interval_ms_ = msec;
  }

  bool isTimeToPublish()
  {
    if(this->callback_interval_ms_ > 0 && millis() - this->last_call_time_ms_ >= this->callback_interval_ms_)
    {
      this->last_call_time_ms_ = millis();
      return true;
    }
    return false;
  }

  bool is_registered_;
  uint32_t pub_msg_cnt_;

private:
  uint32_t last_call_time_ms_;
  uint32_t callback_interval_ms_;
};


class SubscriberHandle
{

public:
  SubscriberHandle()
    : is_registered_(false), topic_id_(0), request_id_(0)
  {
    callback = NULL;
    callback_arg = NULL;
    sub_msg_cnt_ = 0;
  }
  virtual ~SubscriberHandle(){}

  CallbackFunc callback;
  void*        callback_arg;
  virtual void recreate(void) = 0;
  virtual void subscribe(void) = 0;

  virtual void runCallback(void* msg);

  bool is_registered_;
  uint8_t topic_id_;
  uint32_t sub_msg_cnt_;
  uint16_t request_id_;
};

} // namespace ros2



#endif /* ROS2_NODE_HANDLE_HPP_ */
