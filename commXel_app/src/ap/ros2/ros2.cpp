/*
 * ros2.cpp
 *
 *  Created on: Jul 6, 2018
 *      Author: kei
 */


#include "ros2.hpp"

namespace ros2 {
  char* client_communication_method;
  char* server_ip;
  uint16_t server_port;

  static builtin_interfaces::Time synced_time_from_remote;
  static uint32_t millis_when_synced_time;
}

bool ros2::init()
{
  ros2::client_communication_method = (char*)"Serial";

  return true;
}

bool ros2::init(const char* p_server_ip, uint16_t server_port)
{
  ros2::client_communication_method = (char*)"Ethernet";
  ros2::server_ip = (char*)p_server_ip;
  ros2::server_port = server_port;

  return true;
}

void ros2::spin(ros2::Node *node)
{
  node->runPubCallback();

  if(micrortps::runCommunication(1) == false)
  {
    //node->recreate();
  }
}

void ros2::syncTimeFromRemote(builtin_interfaces::Time* msg)
{
  ros2::millis_when_synced_time         = millis();
  ros2::synced_time_from_remote.sec     = msg->sec;
  ros2::synced_time_from_remote.nanosec = msg->nanosec;
}

builtin_interfaces::Time ros2::now()
{
  builtin_interfaces::Time ret_time = synced_time_from_remote;
  uint32_t msec_offset = millis() - ros2::millis_when_synced_time;
  uint32_t remain_msec = msec_offset%1000;

  ret_time.sec += (int32_t)(msec_offset/(uint32_t)1000);
  if((ret_time.nanosec/(uint32_t)1000000 + remain_msec) < 1000)
  {
    ret_time.nanosec += (uint32_t)(remain_msec*(uint32_t)1000000);
  }
  else // >= 1sec
  {
    ret_time.sec += 1;
    ret_time.nanosec = (uint32_t)(ret_time.nanosec + remain_msec*(uint32_t)1000000 - (uint32_t)1000000000);
  }

  return ret_time;
}

const char* ros2::getPrefixString(MessagePrefix prefix)
{
  switch(prefix)
  {
    case ros2::TOPICS_PUBLISH:
      return "rt/";

    case ros2::TOPICS_SUBSCRIBE:
      return "rt/";

    case ros2::SERVICE_REQUEST:
      return "rq/";

    case ros2::SERVICE_RESPONSE:
      return "rr/";

    case ros2::SERVICE:
      return "rs/";

    case ros2::PARAMETER:
      return "rp/";

    case ros2::ACTION:
      return "ra/";
  }

  return "";
}

void ros2::onTopicCallback(mrSession* session, mrObjectId object_id, uint16_t request_id, mrStreamId stream_id, struct MicroBuffer* mb, void* args)
{
  (void)(session); (void)(request_id); (void)(stream_id);
  ros2::Node* node = (ros2::Node*) args;
  uint8_t reader_id = object_id.id;

  node->runSubCallback(reader_id, (void*)mb);
}
