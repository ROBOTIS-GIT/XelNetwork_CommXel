/*
 * xel_net.hpp
 *
 *  Created on: Jul 17, 2018
 *      Author: kei
 */

#ifndef XEL_NET_HPP_
#define XEL_NET_HPP_

#include "xel_net_node.hpp"
#include "hw.h"
#include "dxl/dxl.h"


#define CONNECTED_XEL_MAX 30

namespace XelNetwork
{

enum XelStatus
{
  NOT_CONNECTTED = 0,
  NEW_CONNECTION,
  LOST_CONNECTION,
  RUNNING
};

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
      for(uint8_t i = 0; i < CONNECTED_XEL_MAX; i++)
      {
        switch(xel_tbl[i].status)
        {
          case NEW_CONNECTION:
            createNewTopicWithXel(&node_, &xel_tbl[i]);
            break;
          case LOST_CONNECTION:
            if(xel_tbl[i].is_registered == true)
            {
              //TODO: Delete DDS resource
            }
            memset(&xel_tbl[i], 0, sizeof(xel_tbl));
            connected_xel_cnt--;
            break;
          case RUNNING:
            break;
          case NOT_CONNECTTED:
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
    PlugAndPlay(dxl_t *p_dxl, bool scan_just_init_time, uint32_t interval_ms)
      : flag_scan_just_init_time_(scan_just_init_time)
    {
      p_dxl_ = p_dxl;
      interval_ms_ = interval_ms;
      pre_time_ = 0;
      scanWhenInit();
    }

    void run()
    {
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

  private:
    void scanWhenInit(void)
    {
      //TODO: Broadcast ping
      //if(there is new connected xel)
      xel_tbl[connected_xel_cnt].xel_id = 0; //store new connected xel id
      xel_tbl[connected_xel_cnt++].is_connected = true;
    }

    void scanOneIdEveryInterval(void)
    {
      if(millis() - pre_time_ > interval_ms_)
      {
        pre_time_ = millis();
        //TODO: Individual ping
      }
    }

    void checkConnectedState(void)
    {
      static uint8_t checking_tbl_num = 0;
      bool ret = false;

      if(millis() - pre_time_ > interval_ms_)
      {
        pre_time_ = millis();
        while(checking_tbl_num < CONNECTED_XEL_MAX)
        {
          if(xel_tbl[checking_tbl_num++].is_connected == true)
          {
            //TODO: Individual ping
            p_dxl_->id = xel_tbl[checking_tbl_num].xel_id;
            if(p_dxl_->inst_func.ping(p_dxl_) == DXL_RET_OK)
            {
              ret = true;
            }
            xel_tbl[checking_tbl_num].is_connected = ret;
            break;
          }
        }

        if(checking_tbl_num > CONNECTED_XEL_MAX)
        {
          checking_tbl_num = 0;
        }
      }
    }

    bool flag_scan_just_init_time_;
    uint32_t pre_time_;
    uint32_t interval_ms_;
    dxl_t *p_dxl_;
};

} // namespace XelNetwork

#endif /* XEL_NET_HPP_ */
