/*
 * ap.cpp
 *
 *  Created on: 2017. 2. 13.
 *      Author: baram
 */





#include "ap.hpp"
#include "hw.h"

//-- Internal Variables
//
const volatile  __attribute__((section(".version_str"))) uint8_t fw_version_str[256] = _DEF_APP_VER_STR;
const volatile  __attribute__((section(".version_num"))) uint8_t fw_version_num[256] = _DEF_APP_VER_NUM;


//-- External Variables
//

//-- Internal Functions
static void threadCmdIf(void const * argument);
static void threadButtonLed(void const * argument);
static void threadROS2(void const * argument);

//-- External Functions


void apInit(void)
{
  osThreadId ret;

  /* Setup swtimer */
  timerSetPeriod(_DEF_TIMER1, 1000);
  timerAttachInterrupt(_DEF_TIMER1, swtimerISR);
  timerStart(_DEF_TIMER1);

  cmdifBegin(_DEF_UART1, 57600);
  dxlportOpen(_DEF_DXL1, 1000000);

  /* Begin Ethernet */
#ifdef _USE_HW_ETH
  static uint8_t mac_addr[6] = _HW_DEF_DEFAULT_MAC_ADDR;
  static ip_addr_t ip_addr, subnet, gateway, dns_server;

  /* Static */
  IP4_ADDR(&ip_addr, 192, 168, 0, 4);
  IP4_ADDR(&subnet, 255, 255, 255, 0);
  IP4_ADDR(&gateway, 192, 168, 0, 1);
  IP4_ADDR(&dns_server, 8, 8, 8, 8);

  ethernetIfBegin(IP_DHCP, mac_addr, &ip_addr, &subnet, &gateway, &dns_server);
#endif /* _USE_HW_ETH */

  osThreadDef(threadCmdIf, threadCmdIf, osPriorityNormal, 0, 8*1024/4);
  ret = osThreadCreate (osThread(threadCmdIf), NULL);
  if (ret == NULL)
  {
    cmdifPrintf("osThreadCreate : threadCmdIf fail\n");
  }

  osThreadDef(threadButtonLed, threadButtonLed, osPriorityNormal, 0, 1*1024/4);
  ret = osThreadCreate (osThread(threadButtonLed), NULL);
  if (ret == NULL)
  {
    cmdifPrintf("osThreadCreate : threadButtonLed fail\n");
  }

  osThreadDef(threadROS2, threadROS2, osPriorityNormal, 0, 16*1024/4);
  ret = osThreadCreate (osThread(threadROS2), NULL);
  if (ret == NULL)
  {
    cmdifPrintf("osThreadCreate : threadROS2 fail\n");
  }


  osKernelStart();
}


void apMain(void)
{
  while(1){

  }
}



static void threadCmdIf(void const * argument)
{
  //cmdifPrintf("CMDEV-FW \n");

  for( ;; )
  {
    cmdifMain();
    delay(1);
  }
}

static void threadButtonLed(void const * argument)
{
  delay(500);

  for(;;)
  {
    if (buttonGetPressedEvent(_DEF_BUTTON1))
    {
      ledOn(_DEF_LED1);
    }
    else if (buttonGetReleasedEvent(_DEF_BUTTON1))
    {
      ledOff(_DEF_LED1);
    }
    delay(1);
  }
}


#include "sensor_msgs/BatteryState.hpp"

void on_topic(ObjectId id, MicroBuffer* serialized_topic, void* args);
static bool is_get_BatteryState_topic = false;


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

  void callback()
  {
    if(publisher_ != NULL)
    {
      if(publisher_->isTimeToPublish())
      {
        callbackBatteryStatePub();
      }
    }

    if(is_get_BatteryState_topic)
    {
      if(subscriber_ != NULL)
      {
        subscriber_->subscribe(STREAMID_BUILTIN_RELIABLE);
        is_get_BatteryState_topic = false;
      }
    }
  }

  void callbackBatteryStatePub(void)
  {
    nano_time_ = get_nano_time();

    sensor_msgs::BatteryState battery_state_topic;
    battery_state_topic.header.frame_id = (char*) "CommXel BatteryState";
    battery_state_topic.header.stamp.sec = nano_time_/1000000000;
    battery_state_topic.header.stamp.nanosec = nano_time_%1000000000;

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


  ros2::Publisher<sensor_msgs::BatteryState>* publisher_;
  ros2::Subscriber<sensor_msgs::BatteryState>* subscriber_;

  float cell_voltage_data_[3];
  uint64_t nano_time_;
};


static void threadROS2(void const * argument)
{
#ifdef _USE_HW_ETH
  for(;;)
  {
    if(ethernetGetDhcpStatus() == DHCP_ADDRESS_ASSIGNED)
    {
      break;
    }
  }

  const uint8_t server_ip[4] = {192,168,60,136};  //FIXME
  uint16_t server_port = 2020;  //FIXME

  for( ;; )
  {
    if(ros2::init(server_ip, server_port, on_topic) == true)
    {
      break;
    };
  }
#else
  //Serial
  ros2::init(NULL);
#endif

  uint32_t pre_time = millis();
  BatteryStatePubSub BatteryStateNode;

  for( ;; )
  {
    if(millis() - pre_time > 500)
    {
      pre_time = millis();

      ledToggle(_DEF_LED1);
    }

    ros2::spin(&BatteryStateNode);
  }


  for( ;; )
  {
    osThreadTerminate(NULL);
  }
}


void on_topic(ObjectId id, MicroBuffer* serialized_topic, void* args)
{
  ((void)(args));

  switch(id.data[0])
  {
    case SENSOR_MSGS_BATTERY_STATE_TOPIC:
    {
      sensor_msgs::BatteryState topic;

      topic.deserialize(serialized_topic, &topic);

      is_get_BatteryState_topic = true;

      break;
    }

    default:
      break;
  }
}
