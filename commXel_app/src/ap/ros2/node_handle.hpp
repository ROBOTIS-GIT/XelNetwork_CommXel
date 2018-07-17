/*
 * node_handle.hpp
 *
 *  Created on: May 16, 2018
 *      Author: Kei
 */

#ifndef ROS2_NODE_HANDLE_HPP_
#define ROS2_NODE_HANDLE_HPP_

#include <stdio.h>
#include "micrortps.hpp"


namespace ros2 {

class NodeHandle
{
 
public:
  NodeHandle()
   : last_call_time_us(0), callback_interval_us(0)
  {
  }

  virtual ~NodeHandle(){}

//  virtual void callback(void)
//  {
//    return;
//  }
//  void (*timerCallback)(void);
//  void (*topicCallback)(void);

  virtual void recreate(void) = 0;

  uint32_t last_call_time_us;
  uint32_t callback_interval_us;
};


} // namespace ros2



#endif /* ROS2_NODE_HANDLE_HPP_ */
