/*
 * publisher.hpp
 *
 *  Created on: May 16, 2018
 *      Author: Kei
 */

#ifndef ROS2_PUBLISHER_HPP_
#define ROS2_PUBLISHER_HPP_

#include <stdio.h>
#include "micrortps.hpp"
#include "node_handle.hpp"
#include "topic.hpp"
#include "hw.h"

#define DEFAULT_WRITER_XML ("<profiles><publisher profile_name=\"default_xrce_publisher_profile\"><topic><kind>NO_KEY</kind><name>rt/%s</name><dataType>%s</dataType><historyQos><kind>KEEP_LAST</kind><depth>10</depth></historyQos><durability><kind>TRANSIENT_LOCAL</kind></durability></topic></publisher></profiles>")

extern uint32_t micros(void);


namespace ros2 {

template <typename MsgT>
class Publisher:public NodeHandle
{
 
public:
  Publisher(micrortps::Participant_t* node, const char* name)
    : NodeHandle(), is_registered_(false)
  {
    node_ = node;
    name_ = name;
    this->recreate();
  }

  virtual ~Publisher(){};

  void publish(MsgT * topic, StreamId stream_id)
  {
    if(publisher_.is_init ==  false)
    {
      return;
    }

    topic->writeTopic(node_->session, publisher_.writer_id, stream_id, topic);
  }

  void recreate()
  {
    MsgT topic;
    publisher_.is_init = false;

    char publisher_profile[100] = {0, };
    sprintf(publisher_profile, "<publisher name=\"%s\"", name_);

    char writer_profile[512] = {0, };
    sprintf(writer_profile, DEFAULT_WRITER_XML, name_, topic.type_);
    is_registered_ = micrortps::createPublisher(node_, &publisher_, publisher_profile, writer_profile);
  }

  void setPublishInterval(uint32_t hz)
  {
    this->callback_interval_us = 1000000/hz;
  }

  bool isTimeToPublish()
  {
    if(this->callback_interval_us > 0 && micros() - this->last_call_time_us >= this->callback_interval_us)
    {
      this->last_call_time_us = micros();
      return true;
    }
    return false;
  }

  bool is_registered_;

private:
  const char* name_;
  micrortps::Participant_t* node_;
  micrortps::Publisher_t publisher_;
};


} // namespace ros2



#endif /* ROS2_PUBLISHER_HPP_ */
