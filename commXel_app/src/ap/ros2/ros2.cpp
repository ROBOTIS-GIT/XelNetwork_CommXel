/*
 * ros2.cpp
 *
 *  Created on: Jul 6, 2018
 *      Author: kei
 */


#include "ros2.hpp"


static bool g_is_rmw_init = false;

bool ros2::init(OnTopic callback)
{
  g_is_rmw_init = micrortps::setup(callback);

  return g_is_rmw_init;
}

bool ros2::init(const uint8_t* p_server_ip, uint16_t server_port, OnTopic callback)
{
  g_is_rmw_init = micrortps::setup(p_server_ip, server_port, callback);

  return g_is_rmw_init;
}

void ros2::spin()
{
  micrortps::runCommunication();
}
