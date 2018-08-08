/*
 * ros2.cpp
 *
 *  Created on: Jul 6, 2018
 *      Author: kei
 */


#include "ros2.hpp"


namespace ros2 {
  char* client_communication_method;
  char* server_ip;
  uint16_t server_port;
}

bool ros2::init()
{
  ros2::client_communication_method = (char*)"Serial";

  return true;
}

bool ros2::init(const char* p_server_ip, uint16_t server_port)
{
  ros2::client_communication_method = (char*)"Ethernet";
  ros2::server_ip = (char*)p_server_ip;
  ros2::server_port = server_port;

  return true;
}

void ros2::spin(ros2::Node *node)
{
  node->runPubCallback();

  node->runSession(1000);

//  if(micrortps::runCommunication(1000) == false)
//  {
//    node->recreate();
//  }
}

int64_t ros2::getMillisTime(void)
{
  return micrortps::getMillisTime();
}

builtin_interfaces::Time ros2::now()
{
  builtin_interfaces::Time time;
  int64_t msec = ros2::getMillisTime();

  time.sec = (int32_t)(msec/(int64_t)1000);
  time.nanosec = (uint32_t)((msec%(int64_t)1000)*1000000);

  return time;
}




#include "std_msgs/Bool.hpp"
#include "std_msgs/Empty.hpp"
#include "std_msgs/Header.hpp"
#include "std_msgs/Int32.hpp"
#include "std_msgs/Int64.hpp"
#include "std_msgs/MultiArrayDimension.hpp"
#include "std_msgs/String.hpp"

#include "sensor_msgs/BatteryState.hpp"
#include "sensor_msgs/Imu.hpp"
#include "sensor_msgs/LaserScan.hpp"
#include "sensor_msgs/MagneticField.hpp"
#include "sensor_msgs/JointState.hpp"

#include "nav_msgs/Odometry.hpp"

#include "geometry_msgs/Point.hpp"
#include "geometry_msgs/Pose.hpp"
#include "geometry_msgs/PoseWithCovariance.hpp"
#include "geometry_msgs/Quaternion.hpp"
#include "geometry_msgs/Transform.hpp"
#include "geometry_msgs/TransformStamped.hpp"
#include "geometry_msgs/Twist.hpp"
#include "geometry_msgs/TwistWithCovariance.hpp"
#include "geometry_msgs/Vector3.hpp"

#include "diagnostic_msgs/KeyValue.hpp"

#include "turtlebot3_msgs/SensorState.hpp"
#include "turtlebot3_msgs/Sound.hpp"
#include "turtlebot3_msgs/VersionInfo.hpp"

void onTopicCallback(mrSession* session, mrObjectId object_id, uint16_t request_id, mrStreamId stream_id, struct MicroBuffer* mb, void* args)
{
  (void)(session); (void)(request_id); (void)(stream_id);
  ros2::Node* node = (ros2::Node*) args;
  uint8_t topic_id = object_id.type;

  switch(topic_id)
  {
    case STD_MSGS_BOOL_TOPIC:
    {
      std_msgs::Bool topic;
      topic.deserialize(mb, &topic);
      node->runSubCallback(topic_id, (void*)&topic);
      break;
    }

    case STD_MSGS_STRING_TOPIC:
    {
      std_msgs::String topic;
      topic.deserialize(mb, &topic);
      node->runSubCallback(topic_id, (void*)&topic);
      break;
    }

    case STD_MSGS_INT32_TOPIC:
    {
      std_msgs::Int32 topic;
      topic.deserialize(mb, &topic);
      node->runSubCallback(topic_id, (void*)&topic);
      break;
    }

    case STD_MSGS_INT64_TOPIC:
    {
      std_msgs::Int64 topic;
      topic.deserialize(mb, &topic);
      node->runSubCallback(topic_id, (void*)&topic);
      break;
    }

    case STD_MSGS_EMPTY_TOPIC:
    {
      std_msgs::Empty topic;
      topic.deserialize(mb, &topic);
      node->runSubCallback(topic_id, (void*)&topic);
      break;
    }

    case STD_MSGS_HEADER_TOPIC:
    {
      std_msgs::Header topic;
      topic.deserialize(mb, &topic);
      node->runSubCallback(topic_id, (void*)&topic);
      break;
    }

    case STD_MSGS_MULTI_ARRAY_DIMENSION_TOPIC:
    {
      std_msgs::MultiArrayDimension topic;
      topic.deserialize(mb, &topic);
      node->runSubCallback(topic_id, (void*)&topic);
      break;
    }


    case GEOMETRY_MSGS_VECTOR3_TOPIC:
    {
      geometry_msgs::Vector3 topic;
      topic.deserialize(mb, &topic);
      node->runSubCallback(topic_id, (void*)&topic);
      break;
    }

    case GEOMETRY_MSGS_TWIST_TOPIC:
    {
      geometry_msgs::Twist topic;
      topic.deserialize(mb, &topic);
      node->runSubCallback(topic_id, (void*)&topic);
      break;
    }

    case GEOMETRY_MSGS_QUATERNION_TOPIC:
    {
      geometry_msgs::Quaternion topic;
      topic.deserialize(mb, &topic);
      node->runSubCallback(topic_id, (void*)&topic);
      break;
    }

    case GEOMETRY_MSGS_POINT_TOPIC:
    {
      geometry_msgs::Point topic;
      topic.deserialize(mb, &topic);
      node->runSubCallback(topic_id, (void*)&topic);
      break;
    }

    case GEOMETRY_MSGS_POSE_TOPIC:
    {
      geometry_msgs::Pose topic;
      topic.deserialize(mb, &topic);
      node->runSubCallback(topic_id, (void*)&topic);
      break;
    }

    case GEOMETRY_MSGS_POSE_WITH_CONVARIANCE_TOPIC:
    {
      geometry_msgs::PoseWithCovariance topic;
      topic.deserialize(mb, &topic);
      node->runSubCallback(topic_id, (void*)&topic);
      break;
    }

    case GEOMETRY_MSGS_TWIST_WITH_CONVARIANCE_TOPIC:
    {
      geometry_msgs::TwistWithCovariance topic;
      topic.deserialize(mb, &topic);
      node->runSubCallback(topic_id, (void*)&topic);
      break;
    }

    case GEOMETRY_MSGS_TRANSFORM_TOPIC:
    {
      geometry_msgs::Transform topic;
      topic.deserialize(mb, &topic);
      node->runSubCallback(topic_id, (void*)&topic);
      break;
    }

    case GEOMETRY_MSGS_TRANSFORM_STAMPED_TOPIC:
    {
      geometry_msgs::TransformStamped topic;
      topic.deserialize(mb, &topic);
      node->runSubCallback(topic_id, (void*)&topic);
      break;
    }


    case DIAGNOSTIC_MSGS_KEY_VALUE_TOPIC:
    {
      diagnostic_msgs::KeyValue topic;
      topic.deserialize(mb, &topic);
      node->runSubCallback(topic_id, (void*)&topic);
      break;
    }


    case SENSOR_MSGS_IMU_TOPIC:
    {
      sensor_msgs::Imu topic;
      topic.deserialize(mb, &topic);
      node->runSubCallback(topic_id, (void*)&topic);
      break;
    }

    case SENSOR_MSGS_LASER_SCAN_TOPIC:
    {
      sensor_msgs::LaserScan topic;
      topic.deserialize(mb, &topic);
      node->runSubCallback(topic_id, (void*)&topic);
      break;
    }

    case SENSOR_MSGS_BATTERY_STATE_TOPIC:
    {
      sensor_msgs::BatteryState topic;
      topic.deserialize(mb, &topic);
      node->runSubCallback(topic_id, (void*)&topic);
      break;
    }

    case SENSOR_MSGS_MAGNETIC_FIELD_TOPIC:
    {
      sensor_msgs::MagneticField topic;
      topic.deserialize(mb, &topic);
      node->runSubCallback(topic_id, (void*)&topic);
      break;
    }

    case SENSOR_MSGS_JOINT_STATE_TOPIC:
    {
      sensor_msgs::JointState topic;
      topic.deserialize(mb, &topic);
      node->runSubCallback(topic_id, (void*)&topic);
      break;
    }


    case NAV_MSGS_ODOMETRY_TOPIC:
    {
      nav_msgs::Odometry topic;
      topic.deserialize(mb, &topic);
      node->runSubCallback(topic_id, (void*)&topic);
      break;
    }


    case TURTLEBOT3_MSGS_SOUND_TOPIC:
    {
      turtlebot3_msgs::Sound topic;
      topic.deserialize(mb, &topic);
      node->runSubCallback(topic_id, (void*)&topic);
      break;
    }

    case TURTLEBOT3_MSGS_VERSION_INFO_TOPIC:
    {
      turtlebot3_msgs::VersionInfo topic;
      topic.deserialize(mb, &topic);
      node->runSubCallback(topic_id, (void*)&topic);
      break;
    }

    case TURTLEBOT3_MSGS_SENSOR_STATE_TOPIC:
    {
      turtlebot3_msgs::SensorState topic;
      topic.deserialize(mb, &topic);
      node->runSubCallback(topic_id, (void*)&topic);
      break;
    }

    default:
      break;
  }
}
