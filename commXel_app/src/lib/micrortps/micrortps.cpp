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


//-- Internal Variables
//

bool         g_is_rtps_init_done = false;
Session      g_rtps_session;
SessionId    g_session_id = 0x01;
ClientKey    g_client_key = {{0xAA, 0xBB, 0xCC, 0xDD}};



//-- Internal Functions
//


bool micrortps::setup(OnTopic callback)
{
  if(new_serial_session(&g_rtps_session, g_session_id, g_client_key, "opencr_usb", callback, NULL) == false)
  {
    return false;
  }

  g_is_rtps_init_done = init_session_sync(&g_rtps_session);

  return g_is_rtps_init_done;
}


bool micrortps::setup(const uint8_t* p_server_ip, uint16_t server_port, OnTopic callback)
{
  if(new_udp_session(&g_rtps_session, g_session_id, g_client_key, p_server_ip, server_port, callback, NULL) == false)
  {
    return false;
  }

  g_is_rtps_init_done = init_session_sync(&g_rtps_session);

  return g_is_rtps_init_done;
}


bool micrortps::createParticipant(micrortps::Participant_t* participant)
{
  if(g_is_rtps_init_done == false)
  {
    return false;
  }

  participant->id.data[0] = 0x00;
  participant->id.data[1] = OBJK_PARTICIPANT;
  participant->session = &g_rtps_session;

  participant->is_init = create_participant_sync_by_ref(participant->session, participant->id, "opencr_participant", false, false);
  

  return participant->is_init;
}


bool micrortps::registerTopic(micrortps::Participant_t* participant, char* topic_profile)
{
  if(participant->is_init == false)
  {
    return false;
  }

  bool ret = false;
  uint8_t status;
  ObjectId topic_id = {0x00, OBJK_TOPIC};

  create_topic_sync_by_xml(participant->session, topic_id, topic_profile, participant->id, true, false);
  status = micrortps::getLastStatus();

  if(status == STATUS_OK || status == STATUS_ERR_ALREADY_EXISTS)
  {
    ret = true;
  }

  return ret;
}


bool micrortps::createPublisher(micrortps::Participant_t* participant, micrortps::Publisher_t* publisher, char* publisher_profile, char* writer_profile)
{
  if(participant->is_init == false)
  {
    return false;
  }

  bool ret = false;
  publisher->is_init = false;
  publisher->id.data[0] = 0x00;
  publisher->id.data[1] = OBJK_PUBLISHER;

  ret = create_publisher_sync_by_xml(participant->session, publisher->id, publisher_profile, participant->id, false, true);

  if (ret == true)
  {
    // Create Writer
    publisher->writer_id.data[0] = 0x00;
    publisher->writer_id.data[1] = OBJK_DATAWRITER;

    publisher->is_init = create_datawriter_sync_by_xml(participant->session, publisher->writer_id, writer_profile, publisher->id, false, true);
  }

  return publisher->is_init;
}


bool micrortps::createSubscriber(micrortps::Participant_t* participant, micrortps::Subscriber_t* subscriber, uint8_t topic_id, char* subscriber_profile, char* reader_profile)
{
  if(participant->is_init == false)
  {
    return false;
  }

  bool ret = false;
  subscriber->is_init = false;
  subscriber->id.data[0] = topic_id;
  subscriber->id.data[1] = OBJK_SUBSCRIBER;

  ret = create_subscriber_sync_by_xml(participant->session, subscriber->id, subscriber_profile, participant->id, false, true);

  if (ret == true)
  {
    // Create Writer
    subscriber->reader_id.data[0] = topic_id;
    subscriber->reader_id.data[1] = OBJK_DATAREADER;

    subscriber->is_init = create_datareader_sync_by_xml(participant->session, subscriber->reader_id, reader_profile, subscriber->id, false, true);
  }

  return subscriber->is_init;
}


void micrortps::subscribe(micrortps::Subscriber_t* subscriber, uint8_t StreamId)
{
  if(subscriber->is_init == false)
  {
    return;
  }
  read_data_sync(&g_rtps_session, subscriber->reader_id, StreamId);
}


void micrortps::runCommunication()
{
  run_communication(&g_rtps_session);
}


uint8_t micrortps::getLastStatus()
{
  return g_rtps_session.last_status.status;
}
