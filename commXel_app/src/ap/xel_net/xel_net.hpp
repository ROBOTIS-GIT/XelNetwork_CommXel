/*
 * xel_net.hpp
 *
 *  Created on: Jul 17, 2018
 *      Author: kei
 */

#ifndef XEL_NET_HPP_
#define XEL_NET_HPP_

#include "xel_net_node.hpp"
#include "xels/xels.h"


#define CONNECTED_XEL_MAX 30

namespace XelNetwork
{

static XelInfo_t xel_tbl[CONNECTED_XEL_MAX];
static uint8_t connected_xel_cnt = 0;

class Core
{
  Core()
  {
  };

  public:
    void run(void)
    {
      XelInfo_t* p_xel;

      for(uint8_t i = 0; i < CONNECTED_XEL_MAX; i++)
      {
        p_xel = &xel_tbl[i];
        switch(p_xel->status.current)
        {
          case NEW_CONNECTION:
            createNewTopicWithXel(&node_, p_xel);
            break;

          case LOST_CONNECTION:
            if(p_xel->status.previous == RUNNING)
            {
              //TODO: Delete DDS resource
            }
            memset(p_xel, 0, sizeof(XelInfo_t));
            connected_xel_cnt--;
            p_xel->status.current = NOT_CONNECTTED;
            break;

          case RUNNING:
            //TODO: get data from xel. (p_xel->data)
            //TODO: send data to xel.
            // if(p_xel->flag_get_data == true)
            //    send data(p_xel->data) to xel.
            //    clear p_xel->flag_get_data.
            break;

          case NOT_CONNECTTED:
            //TODO: Idle
            break;

          default:
            break;
        }
      }

      ros2::spin(&node_);
    }

  private:
    XelNetworkNode node_;
};

class PlugAndPlay
{
  public:
    PlugAndPlay(bool scan_just_init_time, uint32_t interval_ms)
      : flag_scan_just_init_time_(scan_just_init_time)
    {
      interval_ms_ = interval_ms;
      pre_time_ = 0;
      scanWhenInit();
    }

    void run()
    {
      if(millis() - pre_time_ > interval_ms_)
      {
        pre_time_ = millis();
        if(flag_scan_just_init_time_ == false)
        {
          if(connected_xel_cnt < CONNECTED_XEL_MAX)
          {
            scanOneIdEveryInterval();
          }
          else
          {
            checkConnectedState();
          }
        }
        else
        {
          checkConnectedState();
        }
      }
    }

  private:
    void scanWhenInit(void)
    {
      //TODO: Broadcast ping
      //if(there is new connected xel)
      // Store corresponding data(XelInfo_t.init_data).

      xelsInit();
    }

    void scanOneIdEveryInterval(void)
    {
      //TODO: Individual ping
    }

    void checkConnectedState(void)
    {
      static uint8_t checking_tbl_num = 0;
      XelInfo_t* p_xel;

      while (checking_tbl_num < CONNECTED_XEL_MAX)
      {
        p_xel = &xel_tbl[checking_tbl_num++];
        if (p_xel->status.current == NEW_CONNECTION
            || p_xel->status.current == RUNNING)
        {
          //TODO: Individual ping
          // if(no ping)
          // p_xel->status.previous = p_xel->status.current;
          // p_xel->status.current = LOST_CONNECTION;
          break;
        }
      }

      if (checking_tbl_num > CONNECTED_XEL_MAX)
      {
        checking_tbl_num = 0;
      }
    }

    bool flag_scan_just_init_time_;
    uint32_t pre_time_;
    uint32_t interval_ms_;
};

} // namespace XelNetwork

#endif /* XEL_NET_HPP_ */
