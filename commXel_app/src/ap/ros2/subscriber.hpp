/*
 * subscriber.hpp
 *
 *  Created on: May 16, 2018
 *      Author: Kei
 */

#ifndef ROS2_SUBSCRIBER_HPP_
#define ROS2_SUBSCRIBER_HPP_

#include <stdio.h>
#include "micrortps.hpp"
#include "node_handle.hpp"
#include "topic.hpp"

#define DEFAULT_READER_XML ("<profiles><subscriber profile_name=\"default_xrce_subscriber_profile\"><topic><kind>NO_KEY</kind><name>rt/%s</name><dataType>%s</dataType><historyQos><kind>KEEP_LAST</kind><depth>10</depth></historyQos><durability><kind>TRANSIENT_LOCAL</kind></durability></topic></subscriber></profiles>")


namespace ros2
{

class Node;

template <typename MsgT>
class Subscriber:public NodeHandle
{

  public:
    Subscriber(micrortps::Participant_t* node, const char* name)
      : NodeHandle(), is_registered_(false)
    {
      MsgT topic;

      node_ = node;
      name_ = name;
      topic_id_ = topic.id_;
      this->recreate();
    }

    virtual ~Subscriber(){};

    void subscribe(StreamId stream_id)
    {
      if(subscriber_.is_init ==  false)
      {
        return;
      }

      micrortps::subscribe(&subscriber_, stream_id);
    }

    void recreate()
    {
      MsgT topic;

      subscriber_.is_init = false;

      char subscriber_profile[100] = {0, };
      sprintf(subscriber_profile, "<subscriber name=\"%s\"", name_);

      char reader_profile[512] = {0, };
      sprintf(reader_profile, DEFAULT_READER_XML, name_, topic.type_);
      is_registered_ = micrortps::createSubscriber(node_, &subscriber_, topic_id_, subscriber_profile, reader_profile);
    };  

    bool is_registered_;
    uint8_t topic_id_;

  private:
    const char* name_;
    micrortps::Participant_t* node_;
    micrortps::Subscriber_t subscriber_;
};

} // namespace ros2

#endif /* ROS2_SUBSCRIBER_HPP_ */
