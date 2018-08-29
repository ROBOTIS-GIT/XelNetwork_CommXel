/*
 * ros2.hpp
 *
 *  Created on: May 16, 2018
 *      Author: Kei
 */

#ifndef MICRORTPS_HPP_
#define MICRORTPS_HPP_


#include "microcdr/microcdr.h"
#include "micrortps/client/client.h"
#include <micrortps/client/core/serialization/xrce_protocol.h>
#include <micrortps/client/core/session/submessage.h>


#ifdef PROFILE_UDP_TRANSPORT
#if defined(PLATFORM_NAME_COMMXEL)
#include <transport/udp_transport_commxel.h>
#endif
#endif //PROFILE_UDP_TRANSPORT

#ifdef PROFILE_TCP_TRANSPORT
#if defined(PLATFORM_NAME_COMMXEL)
#include <transport/tcp_transport_commxel.h>
#endif
#endif //PROFILE_TCP_TRANSPORT

#ifdef PROFILE_SERIAL_TRANSPORT
#if defined(PLATFORM_NAME_OPENCR)
#include <transport/serial_transport_opencr.h>
#elif defined(PLATFORM_NAME_COMMXEL)
#include <transport/serial_transport_commxel.h>
#endif
#endif //PROFILE_SERIAL_TRANSPORT


namespace micrortps {

typedef struct Participant{
  bool is_init;
  mrObjectId id;
  mrSession *session;
  mrStreamId output_stream_id;
  mrStreamId input_stream_id;
} Participant_t;

typedef struct Subscriber{
  bool is_init;
  mrObjectId id;
  mrObjectId reader_id;
  uint16_t read_req;
  mrDeliveryControl delivery_control;
  Participant_t *participant;
} Subscriber_t;

typedef struct Publisher{
  bool is_init;
  mrObjectId id;
  mrObjectId writer_id;
  Participant_t *participant;
} Publisher_t;




bool setup(mrOnTopicFunc callback, void* callback_arg);
bool setup(const char* p_server_ip, uint16_t server_port, mrOnTopicFunc callback, void* callback_arg);
bool createSession(mrCommunication* comm, mrOnTopicFunc callback, void* callback_arg);
bool createParticipant(Participant_t* participant);
bool registerTopic(Participant_t* participant, char* topic_profile, uint8_t topic_id);
bool createPublisher(Participant_t* participant, Publisher_t* publisher, char* publisher_profile, char* writer_profile);
bool createSubscriber(Participant_t* participant, Subscriber_t* subscriber, char* subscriber_profile, char* reader_profile);
void subscribe(Subscriber_t* subscriber);
bool runCommunication(uint32_t timeout_ms);
int64_t getMillisTime(void);


} /* namespace micrortps */



#endif /* ROS2_HPP_ */
