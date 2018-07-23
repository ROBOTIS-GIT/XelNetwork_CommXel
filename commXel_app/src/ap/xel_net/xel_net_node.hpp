/*
 * xel_net_node.hpp
 *
 *  Created on: Jul 17, 2018
 *      Author: kei
 */

#ifndef XEL_NET_NODE_HPP_
#define XEL_NET_NODE_HPP_

#include "ros2.hpp"
#include "sensor_msgs/BatteryState.hpp"


#define BATTERY_STATE_PUBLISH_FREQUENCY 2 //hz

void publishBatteryState(sensor_msgs::BatteryState* msg);
void subscribeBatteryState(sensor_msgs::BatteryState* msg);


class BatteryStatePubSub : public ros2::Node
{
public:
  BatteryStatePubSub()
  : Node()
  {
    publisher_ = this->createPublisher<sensor_msgs::BatteryState>("BatteryState");
    this->createWallFreq(BATTERY_STATE_PUBLISH_FREQUENCY, (ros2::CallbackFunc)publishBatteryState, publisher_);

    subscriber_ = this->createSubscriber<sensor_msgs::BatteryState>("BatteryState", (ros2::CallbackFunc)subscribeBatteryState);
  }


private:
  ros2::Publisher<sensor_msgs::BatteryState>* publisher_;
  ros2::Subscriber<sensor_msgs::BatteryState>* subscriber_;
};



void publishBatteryState(sensor_msgs::BatteryState* msg)
{
      static float cell_voltage_data_[3];

      msg->header.frame_id          = (char*) "OpenCR BatteryState";
      msg->header.stamp             = ros2::now();

      msg->voltage                  = 1;
      msg->current                  = 2;
      msg->charge                   = 3;
      msg->capacity                 = 4;
      msg->design_capacity          = 5;
      msg->percentage               = 6;

      msg->power_supply_status      = 0;
      msg->power_supply_health      = 1;
      msg->power_supply_technology  = 2;

      msg->present                  = true;
      msg->cell_voltage             = cell_voltage_data_;
      msg->cell_voltage_size        = sizeof(cell_voltage_data_)/sizeof(float);
      for(uint32_t i = 0; i < msg->cell_voltage_size; i++)
      {
        msg->cell_voltage[i]        = 3.7;
      }

      msg->location                 = (char*)"Seoul";
      msg->serial_number            = (char*)"123-456-789";
}


void subscribeBatteryState(sensor_msgs::BatteryState* msg)
{

}



#endif /* XEL_NET_NODE_HPP_ */
