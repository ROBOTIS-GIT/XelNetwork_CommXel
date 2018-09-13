// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


/*! 
 * @file JointState.hpp
 * This header file contains the declaration of the described types in the IDL file.
 *
 * This file was generated by the tool gen.
 */
#ifndef _SENSOR_MSGS_JOINT_STATE_HPP_
#define _SENSOR_MSGS_JOINT_STATE_HPP_


#include "micrortps.hpp"
#include <topic_config.h>
#include <topic.hpp>


#include "std_msgs/Header.hpp"

namespace sensor_msgs {

class JointState : public ros2::Topic<JointState>
{
public: 
    std_msgs::Header header;
    char name[255][10];
    uint32_t name_size;
    double position[255];
    uint32_t position_size;
    double velocity[255];
    uint32_t velocity_size;
    double effort[255];
    uint32_t effort_size;

  JointState():
    Topic("sensor_msgs::msg::dds_::JointState_", SENSOR_MSGS_JOINT_STATE_TOPIC),
    header(),
    name_size(1), position_size(1), velocity_size(1), effort_size(1)
  { 
    memset(name, 0, sizeof(name));
    memset(position, 0, sizeof(position));
    memset(velocity, 0, sizeof(velocity));
    memset(effort, 0, sizeof(effort));
  }

  bool serialize(struct MicroBuffer* writer, const JointState* topic)
  {
    (void) header.serialize(writer, &topic->header);
    
    (void) serialize_uint32_t(writer, topic->name_size);
    for(uint32_t i = 0; i < topic->name_size; i++)
    {
      (void) serialize_sequence_char(writer, topic->name[i], (uint32_t)(strlen(topic->name[i])+1));
    } 

    (void) serialize_sequence_double(writer, topic->position, topic->position_size);
    (void) serialize_sequence_double(writer, topic->velocity, topic->velocity_size);
    (void) serialize_sequence_double(writer, topic->effort, topic->effort_size);

    return writer->error == BUFFER_OK;
  }

  bool deserialize(struct MicroBuffer* reader, JointState* topic)
  {
    (void) header.deserialize(reader, &topic->header);
    
    uint32_t size_string = 0, size_data;
    (void) deserialize_uint32_t(reader, &size_string);
    for(uint32_t i = 0; i < size_string; i++)
    {
      (void) deserialize_sequence_char(reader, topic->name[i], &size_data);
    }

    (void) deserialize_sequence_double(reader, topic->position, &topic->position_size);
    (void) deserialize_sequence_double(reader, topic->velocity, &topic->velocity_size);
    (void) deserialize_sequence_double(reader, topic->effort, &topic->effort_size);

    return reader->error == BUFFER_OK;
  }

  virtual uint32_t size_of_topic(const JointState* topic, uint32_t size)
  {
    size  = header.size_of_topic(&topic->header, size);

    size += 4 + get_alignment(size, 4);
    for(uint32_t i = 0; i < name_size; i++)
    {
      size += 4 + get_alignment(size, 4) + (uint32_t)(strlen(name[i]) + 1);
    }

    size += 4 + get_alignment(size, 4);
    size += (topic->position_size * 8) + get_alignment(size, 8);

    size += 4 + get_alignment(size, 4);
    size += (topic->velocity_size * 8) + get_alignment(size, 8);

    size += 4 + get_alignment(size, 4);
    size += (topic->effort_size * 8) + get_alignment(size, 8);


    return size;
  }

};

} // namespace sensor_msgs


#endif // _SENSOR_MSGS_JOINT_STATE_HPP_
