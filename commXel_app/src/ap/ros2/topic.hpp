/*
 * topic.hpp
 *
 *  Created on: May 16, 2018
 *      Author: Kei
 */

#ifndef ROS2_TOPIC_HPP_
#define ROS2_TOPIC_HPP_

#include <stdlib.h>
#include "micrortps.hpp"

#define DEFAULT_TOPIC_XML ("<dds><topic><name>rt/%s</name><dataType>%s</dataType></topic></dds>")

namespace ros2 {

/* Base Message Type */
template <typename MsgT>
class Topic
{

public:

  Topic(const char* type, uint8_t id):
    type_(type),
    id_(id)
  {
  }

  virtual ~Topic()
  {
  }

  virtual bool serialize(MicroBuffer* writer, const MsgT* topic) = 0;
  virtual bool deserialize(MicroBuffer* reader, MsgT* topic) = 0;

  virtual bool writeTopic(Session* session, ObjectId datawriter_id, StreamId stream_id, MsgT* topic)
  {
    if (session == NULL)
    {
      return false;
    }

    bool result = false;
    uint32_t topic_size = size_of_topic(topic);
    MicroBuffer* topic_buffer = NULL;

    if (128 < stream_id)
    {
      topic_buffer = prepare_best_effort_stream_for_topic(&session->output_best_effort_stream, datawriter_id, topic_size);
    }
    else
    {
      topic_buffer = prepare_reliable_stream_for_topic(&session->output_reliable_stream, datawriter_id, topic_size);
    }

    if (topic_buffer != NULL)
    {
      result = serialize(topic_buffer, topic);
    }

    return result;
  }

  virtual uint32_t size_of_topic(const MsgT* topic)
  {
    (void)(topic);

    return 0;
  }

  const char* type_;
  uint8_t id_;
};



} // namespace ros2



#endif /* ROS2_TOPIC_HPP_ */
