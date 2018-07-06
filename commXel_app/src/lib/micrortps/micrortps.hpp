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
#include "micrortps/client/xrce_protocol_spec.h"



namespace micrortps {



typedef struct Subscriber{
  bool is_init;
  ObjectId id;
  ObjectId reader_id;
} Subscriber_t;

typedef struct Publisher{
  bool is_init;
  ObjectId id;
  ObjectId writer_id;
} Publisher_t;

typedef struct Participant{
  bool is_init;
  ObjectId id;
  Session *session;
} Participant_t;


bool setup(OnTopic callback);
bool setup(const uint8_t* p_server_ip, uint16_t server_port, OnTopic callback);
bool createParticipant(Participant_t* participant);
bool registerTopic(Participant_t* participant, char* topic_profile);
bool createPublisher(Participant_t* participant, Publisher_t* publisher, char* publisher_profile, char* writer_profile);
bool createSubscriber(Participant_t* participant, Subscriber_t* subscriber, uint8_t topic_id, char* subscriber_profile, char* reader_profile);
void subscribe(Subscriber_t* subscriber, uint8_t StreamId);
void runCommunication(void);
uint8_t getLastStatus(void);



} /* namespace micrortps */



#endif /* ROS2_HPP_ */
