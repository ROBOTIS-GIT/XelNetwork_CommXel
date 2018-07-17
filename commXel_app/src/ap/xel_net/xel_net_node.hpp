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

class BatteryStatePubSub : public ros2::Node
{
public:
  BatteryStatePubSub()
  : Node(), nano_time_(0)
  {
    memset(cell_voltage_data_, 0, sizeof(cell_voltage_data_));

    publisher_ = this->createPublisher<sensor_msgs::BatteryState>("BatteryState");
    if(publisher_ != NULL)
    {
      publisher_->setPublishInterval(2); // 2 hz
    }

    subscriber_ = this->createSubscriber<sensor_msgs::BatteryState>("BatteryState");
    if(subscriber_ != NULL)
    {
      subscriber_->subscribe(STREAMID_BUILTIN_RELIABLE);
    }
  }


private:

  void timerCallback()
  {
    if(publisher_->isTimeToPublish())
    {
      nano_time_ = get_nano_time();

      sensor_msgs::BatteryState battery_state_topic;
      battery_state_topic.header.frame_id = (char*) "CommXel BatteryState";
      battery_state_topic.header.stamp.sec = nano_time_/(uint64_t)1000000000;
      battery_state_topic.header.stamp.nanosec = nano_time_%(uint64_t)1000000000;

      battery_state_topic.voltage = 1;
      battery_state_topic.current = 2;
      battery_state_topic.charge = 3;
      battery_state_topic.capacity = 4;
      battery_state_topic.design_capacity = 5;
      battery_state_topic.percentage = 6;

      battery_state_topic.power_supply_status = 0;
      battery_state_topic.power_supply_health = 1;
      battery_state_topic.power_supply_technology = 2;

      battery_state_topic.present = true;
      battery_state_topic.cell_voltage = cell_voltage_data_;
      battery_state_topic.cell_voltage_size = sizeof(cell_voltage_data_)/sizeof(float);
      for(uint32_t i = 0; i < battery_state_topic.cell_voltage_size; i++)
      {
        battery_state_topic.cell_voltage[i] = 3.7;
      }

      battery_state_topic.location = (char*)"Seoul";
      battery_state_topic.serial_number = (char*)"123-456-789";

      if(publisher_ != NULL)
      {
        publisher_->publish(&battery_state_topic, STREAMID_BUILTIN_RELIABLE);
      }
    }
  }

  void userTopicCallback(uint8_t topic_id, void* topic_msg)
  {
    if(topic_id == subscriber_->topic_id_)
    {
      subscriber_->subscribe(STREAMID_BUILTIN_RELIABLE);
    }
  }

  ros2::Publisher<sensor_msgs::BatteryState>* publisher_;
  ros2::Subscriber<sensor_msgs::BatteryState>* subscriber_;

  float cell_voltage_data_[3];
  uint64_t nano_time_;
};




#endif /* XEL_NET_NODE_HPP_ */
