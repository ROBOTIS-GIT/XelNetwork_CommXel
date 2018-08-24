/*
 * xel_net_msgs.cpp
 *
 *  Created on: Aug 22, 2018
 *      Author: kei
 */

#include "xel_net_msgs.hpp"
#include "xel_net_struct.hpp"


using namespace XelNetwork;




/////////////////////////////////////////////////////////////
///// Send Message Callback
/////////////////////////////////////////////////////////////
void sendMessage(std_msgs::Bool* msg, void* arg)
{
  Bool_t *p_data = (Bool_t*)arg;
  msg->data = p_data->data;
}




/////////////////////////////////////////////////////////////
///// Received Message Callback
/////////////////////////////////////////////////////////////







