/*
 * xel_net_node.hpp
 *
 *  Created on: Jul 17, 2018
 *      Author: kei
 */

#ifndef XEL_NET_NODE_HPP_
#define XEL_NET_NODE_HPP_

#include "ros2.hpp"
#include <sensor_msgs/Imu.hpp>
#include <sensor_msgs/BatteryState.hpp>
#include <sensor_msgs/LaserScan.hpp>
#include <std_msgs/Empty.hpp>
#include <std_msgs/Bool.hpp>


#define BATTERY_STATE_PUBLISH_FREQUENCY     200    //hz
#define LASER_SCAN_PUBLISH_FREQUENCY        200    //hz
#define IMU_PUBLISH_FREQUENCY               200    //hz




void publishBatteryState(sensor_msgs::BatteryState* topic_msg);
void subscribeBatteryState(sensor_msgs::BatteryState* topic_msg);

void publishLaserScan(sensor_msgs::LaserScan* msg);
void subscribeLaserScan(sensor_msgs::LaserScan* msg);

void publishImu(sensor_msgs::Imu* msg);
void subscribeImu(sensor_msgs::Imu* msg);

void subscribeReset(std_msgs::Empty* msg);
void subscribeLed(std_msgs::Bool* msg);

namespace XelNetwork {

class XelNetworkNode : public ros2::Node
{
public:
    XelNetworkNode()
  : Node()
  {
    commxel_battery_pub_ = this->createPublisher<sensor_msgs::BatteryState>("commXel_battery");
    this->createWallFreq(BATTERY_STATE_PUBLISH_FREQUENCY, (ros2::CallbackFunc)publishBatteryState, commxel_battery_pub_);
    commxel_battery_sub_= this->createSubscriber<sensor_msgs::BatteryState>("BatteryState", (ros2::CallbackFunc)subscribeBatteryState);

    laser_scan_pub_ = this->createPublisher<sensor_msgs::LaserScan>("commXel_laser");
    this->createWallFreq(LASER_SCAN_PUBLISH_FREQUENCY, (ros2::CallbackFunc)publishLaserScan, laser_scan_pub_);
    laser_scan_sub_ = this->createSubscriber<sensor_msgs::LaserScan>("commXel_laser", (ros2::CallbackFunc)subscribeLaserScan);

    imu_pub_ = this->createPublisher<sensor_msgs::Imu>("commXel_imu");
    this->createWallFreq(IMU_PUBLISH_FREQUENCY, (ros2::CallbackFunc)publishImu, imu_pub_);
    imu_sub_ = this->createSubscriber<sensor_msgs::Imu>("commXel_imu", (ros2::CallbackFunc)subscribeImu);

    reset_sub_ = this->createSubscriber<std_msgs::Empty>("commXel_reset", (ros2::CallbackFunc)subscribeReset);
    led_sub    = this->createSubscriber<std_msgs::Bool>("commXel_led", (ros2::CallbackFunc)subscribeLed);
  }


private:
  ros2::Publisher<sensor_msgs::BatteryState>*  commxel_battery_pub_;
  ros2::Subscriber<sensor_msgs::BatteryState>* commxel_battery_sub_;

  ros2::Publisher<sensor_msgs::LaserScan>*     laser_scan_pub_;
  ros2::Subscriber<sensor_msgs::LaserScan>*    laser_scan_sub_;

  ros2::Publisher<sensor_msgs::Imu>*           imu_pub_;
  ros2::Subscriber<sensor_msgs::Imu>*          imu_sub_;

  ros2::Subscriber<std_msgs::Empty>*           reset_sub_;
  ros2::Subscriber<std_msgs::Bool>*            led_sub;
};

} // namespace XelNetwork


void publishBatteryState(sensor_msgs::BatteryState* msg)
{
  static float cell_voltage_data_[3];

  msg->header.frame_id          = (char*) "CommXel BatteryState";
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

  msg->location                 = (char*)"PowerXel";
  msg->serial_number            = (char*)"123-456-789";
}

void subscribeBatteryState(sensor_msgs::BatteryState* msg)
{

}



void publishLaserScan(sensor_msgs::LaserScan* msg)
{
  float ranges_data_[8];
  float intensities_data_[8];

  msg->header.frame_id  = (char*) "CommXel LaserScan";
  msg->header.stamp     = ros2::now();

  msg->angle_min        = 1;
  msg->angle_max        = 2;
  msg->angle_increment  = 3;
  msg->time_increment   = 4;
  msg->scan_time        = 5;
  msg->range_min        = 6;
  msg->range_max        = 7;

  msg->ranges           = ranges_data_;
  msg->ranges_size      = sizeof(ranges_data_)/sizeof(float);
  for(uint32_t i = 0; i < msg->ranges_size; i++)
  {
    msg->ranges[i]      = (float)(micros()%128);
  }

  msg->intensities      = intensities_data_;
  msg->intensities_size = sizeof(intensities_data_)/sizeof(float);
  for(uint32_t i = 0; i < msg->intensities_size; i++)
  {
    msg->intensities[i] = (float)(micros()%128);
  }
}


void subscribeLaserScan(sensor_msgs::LaserScan* msg)
{

}



void publishImu(sensor_msgs::Imu* msg)
{
  msg->header.frame_id       = (char*) "CommXel IMU";
  msg->header.stamp          = ros2::now();

  msg->orientation.x         = 1;
  msg->orientation.y         = 2;
  msg->orientation.z         = 3;
  msg->orientation.w         = 4;
  msg->angular_velocity.x    = 5;
  msg->angular_velocity.y    = 6;
  msg->angular_velocity.z    = 7;
  msg->linear_acceleration.x = 8;
  msg->linear_acceleration.y = 9;
  msg->linear_acceleration.z = 10;
}


void subscribeImu(sensor_msgs::Imu* msg)
{

}


void subscribeReset(std_msgs::Empty* msg)
{

}

void subscribeLed(std_msgs::Bool* msg)
{
  if(msg->data == true)
  {
    ledOn(_DEF_LED1);
  }
  else
  {
    ledOff(_DEF_LED1);
  }
}


#endif /* XEL_NET_NODE_HPP_ */
