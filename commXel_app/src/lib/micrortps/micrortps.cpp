/*
 *  micrortps.cpp
 *
 *  Created on: 2018. 5. 25.
 *      Author: Kei
 */

#include <stdlib.h>
#include <string.h>

#include "micrortps.hpp"
#include "hw.h"
#include <micrortps/client/core/util/time.h>


//-- Internal Variables
//
#define STREAM_HISTORY  8
#define BUFFER_SIZE     MR_CONFIG_UDP_TRANSPORT_MTU * STREAM_HISTORY

bool         g_is_rtps_init_done = false;
uint32_t     g_session_key = 0xAABBCCDD;
mrSession    g_rtps_session;


uint8_t      output_reliable_stream_buffer[BUFFER_SIZE];
uint8_t      input_reliable_stream_buffer[BUFFER_SIZE];

//-- Internal Functions
//


bool micrortps::setup(mrOnTopicFunc callback, void* callback_arg)
{
  // Transport
  static mrSerialTransport transport;
  if(mr_init_uart_transport(&transport, "usb", 0, 0) == false)
  {
    return false;
  }

  // Session
  mr_init_session(&g_rtps_session, &transport.comm, g_session_key);
  mr_set_topic_callback(&g_rtps_session, callback, callback_arg);

  g_is_rtps_init_done = mr_create_session(&g_rtps_session);

  return g_is_rtps_init_done;
}


bool micrortps::setup(const char* p_server_ip, uint16_t server_port, mrOnTopicFunc callback, void* callback_arg)
{
#if defined(PROFILE_UDP_TRANSPORT)  
  // Transport
  static mrUDPTransport transport;
  if(mr_init_udp_transport(&transport, p_server_ip, server_port) == false)
  {
    return false;
  }

  // Session
  mr_init_session(&g_rtps_session, &transport.comm, g_session_key);
  mr_set_topic_callback(&g_rtps_session, callback, callback_arg);

  g_is_rtps_init_done = mr_create_session(&g_rtps_session);

#elif defined(PROFILE_TCP_TRANSPORT)
  (void)(p_server_ip); (void)(server_port); (void)(callback); (void)(callback_arg);
#else
  (void)(p_server_ip); (void)(server_port); (void)(callback); (void)(callback_arg);
#endif
  return g_is_rtps_init_done;
}


bool micrortps::createParticipant(micrortps::Participant_t* participant)
{
  if(g_is_rtps_init_done == false)
  {
    return false;
  }

  participant->reliable_out = mr_create_output_reliable_stream(&g_rtps_session, output_reliable_stream_buffer, BUFFER_SIZE, STREAM_HISTORY);
  participant->reliable_in  = mr_create_input_reliable_stream(&g_rtps_session, input_reliable_stream_buffer, BUFFER_SIZE, STREAM_HISTORY);
  participant->session = &g_rtps_session;

  static uint8_t object_id = 0x00;
  participant->id = mr_object_id(object_id++, MR_PARTICIPANT_ID);
  const char* participant_ref = (char*)"default participant";
  uint16_t participant_req = mr_write_create_participant_ref(&g_rtps_session, participant->reliable_out, participant->id, 0, participant_ref, MR_REPLACE);

  uint8_t status;
  participant->is_init = mr_run_session_until_status(participant->session, 1000, &participant_req, &status, 1);

  return participant->is_init;
}


bool micrortps::registerTopic(micrortps::Participant_t* participant, char* topic_profile, uint8_t topic_id)
{
  if(participant->is_init == false)
  {
    return false;
  }

  uint8_t status;
  bool ret = false;

  mrObjectId object_id = mr_object_id(topic_id, MR_TOPIC_ID);

  uint16_t topic_req = mr_write_configure_topic_xml(participant->session, participant->reliable_out, object_id, participant->id, topic_profile, MR_REUSE);
  ret = mr_run_session_until_status(participant->session, 1000, &topic_req, &status, 1);

  return ret;
}


bool micrortps::createPublisher(micrortps::Participant_t* participant, micrortps::Publisher_t* publisher, uint8_t topic_id, char* publisher_profile, char* writer_profile)
{
  if(participant->is_init == false)
  {
    return false;
  }

  bool ret                 = false;
  static uint8_t pub_id    = 0x00;
  publisher->is_init       = false;
  publisher->id            = mr_object_id(pub_id++, MR_PUBLISHER_ID);
  publisher->participant   = participant;

  uint8_t status;

  uint16_t publisher_req = mr_write_configure_publisher_xml(participant->session, participant->reliable_out, publisher->id, participant->id, publisher_profile, MR_REPLACE);
  ret = mr_run_session_until_status(participant->session, 1000, &publisher_req, &status, 1);

  if (ret == true)
  {
    // Create Writer
    //static uint8_t writer_id     = 0x00;
    publisher->writer_id = mr_object_id(topic_id, MR_DATAWRITER_ID);
    uint16_t datawriter_req = mr_write_configure_datawriter_xml(participant->session, participant->reliable_out, publisher->writer_id, publisher->id, writer_profile, MR_REPLACE);

    publisher->is_init = mr_run_session_until_status(participant->session, 1000, &datawriter_req, &status, 1);
  }

  return publisher->is_init;
}


bool micrortps::createSubscriber(micrortps::Participant_t* participant, micrortps::Subscriber_t* subscriber, uint8_t topic_id, char* subscriber_profile, char* reader_profile)
{
  if(participant->is_init == false)
  {
    return false;
  }

  bool ret                  = false;
  static uint8_t sub_id     = 0x00;
  subscriber->is_init       = false;
  subscriber->id            = mr_object_id(sub_id++, MR_SUBSCRIBER_ID);
  subscriber->participant   = participant;

  uint8_t status;
  uint16_t subscriber_req = mr_write_configure_subscriber_xml(participant->session, participant->reliable_out, subscriber->id, participant->id, subscriber_profile, MR_REPLACE);
  ret = mr_run_session_until_status(participant->session, 1000, &subscriber_req, &status, 1);

  if (ret == true)
  {
    // Create Writer
    //static uint8_t writer_id     = 0x00;
    subscriber->reader_id = mr_object_id(topic_id, MR_DATAREADER_ID);
    uint16_t datareader_req = mr_write_configure_datareader_xml(participant->session, participant->reliable_out, subscriber->reader_id, subscriber->id, reader_profile, MR_REPLACE);

    subscriber->is_init = mr_run_session_until_status(participant->session, 1000, &datareader_req, &status, 1);
  }

  return subscriber->is_init;
}


void micrortps::subscribe(micrortps::Subscriber_t* subscriber)
{
  if(subscriber->is_init == false)
  {
    return;
  }
  subscriber->read_req = mr_write_request_data(subscriber->participant->session, subscriber->participant->reliable_out, subscriber->reader_id, subscriber->participant->reliable_in, &subscriber->delivery_control);
}


bool micrortps::runCommunication(uint32_t timeout_ms)
{
  bool is_connected;
  is_connected = mr_run_session_until_timeout(&g_rtps_session, timeout_ms);
  return is_connected;
}


int64_t micrortps::getMillisTime(void)
{
  return get_milli_time();
}

