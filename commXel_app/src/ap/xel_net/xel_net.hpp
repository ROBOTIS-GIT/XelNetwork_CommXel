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


#define CONNECTED_XEL_MAX      30


extern osSemaphoreId dxl_semaphore;

namespace XelNetwork
{

static XelInfo_t xel_tbl[CONNECTED_XEL_MAX];
static uint8_t connected_xel_cnt = 0;

class Core
{
  public:
    Core()
    {
    };

    void run(void)
    {
      XelInfo_t* p_xel;

      for(uint8_t i = 0; i < CONNECTED_XEL_MAX; i++)
      {
        p_xel = &xel_tbl[i];
        switch(p_xel->status.current)
        {
          case NEW_CONNECTION:
            if(createNewTopicWithXel(&node_, p_xel) == true)
            {
              p_xel->status.previous = p_xel->status.current;
              p_xel->status.current = RUNNING;
            }
            break;

          case LOST_CONNECTION:
            if(p_xel->status.previous == RUNNING)
            {
              deleteDDSResource(p_xel);
            }
            memset(p_xel, 0, sizeof(XelInfo_t));
            p_xel->status.current = NOT_CONNECTTED;
            break;

          case RUNNING:
            if(p_xel->data_direction == XelNetwork::SEND)
            {
              if(osSemaphoreWait(dxl_semaphore, 0) == osOK)
              {
                xelsReadData(p_xel);
                osSemaphoreRelease(dxl_semaphore);
              }
            }
            else // send data to xel //XelNetwork::RECEIVE
            {
              if(p_xel->status.flag_get_data == true)
              {
                if(osSemaphoreWait(dxl_semaphore, 0) == osOK)
                {
                  //TODO: send data to xel
                  osSemaphoreRelease(dxl_semaphore);
                  p_xel->status.flag_get_data = false;
                }
              }
            }

            break;

          case NOT_CONNECTTED:
            //TODO: Idle
            break;

          default:
            break;
        }

        ros2::spin(&node_);
      }
    }

  private:
    void deleteDDSResource(XelInfo_t* p_xel)
    {
      if(p_xel->data_direction == XelNetwork::SEND)
      {
        node_.deleteWriter(p_xel->dds.entity_id);
      }
      else
      {
        node_.deleteReader(p_xel->dds.entity_id);
      }
    }

    XelNetworkNode node_;
};

class PlugAndPlay
{
  public:
    PlugAndPlay(bool scan_just_init_time, uint32_t interval_ms)
      : flag_scan_just_init_time_(scan_just_init_time)
    {
      interval_ms_ = interval_ms;

      xelsInit();
      xelsOpen(_DEF_DXL1, 1000000);
      scanWhenInit();
    }

    void run()
    {
      if(osSemaphoreWait(dxl_semaphore, interval_ms_) == osOK)
      {
        scanIdEveryInterval();
        osSemaphoreRelease(dxl_semaphore);
      }
    }

  private:
    void scanWhenInit(void)
    {
      if(osSemaphoreWait(dxl_semaphore, 1000) == osOK)
      {
        connected_xel_cnt = xelsPings(xel_tbl, CONNECTED_XEL_MAX);
        osSemaphoreRelease(dxl_semaphore);
      }

      for(uint8_t i = 0; i < connected_xel_cnt; i++)
      {
        xel_tbl[i].status.current = NEW_CONNECTION;
      }
    }

    void scanIdEveryInterval(void)
    {
      XelInfo_t* p_xel;
      static uint32_t pre_time = 0;
      static uint8_t checking_tbl_num = 0;

      if (millis() - pre_time > interval_ms_)
      {
        pre_time = millis();
        p_xel = &xel_tbl[checking_tbl_num++];

        if (p_xel->status.current == RUNNING || p_xel->status.current == NEW_CONNECTION)
        {
          if (xelsPing(p_xel) == false)
          {
            p_xel->status.previous = p_xel->status.current;
            p_xel->status.current = LOST_CONNECTION;
            connected_xel_cnt--;
          }
        }
        else //new id scan
        {
          //select id for scan
          static uint8_t id = _XELS_START_ID;
          bool ret = true;

          for(uint8_t i = 0; i < CONNECTED_XEL_MAX; i++)
          {
            if(xel_tbl[i].xel_id == id && xel_tbl[i].status.current != NOT_CONNECTTED)
            {
              ret = false;
              break;
            }
          }

          if(ret == true)
          {
            p_xel->xel_id = id;
            ret = xelsPing(p_xel);

            if (ret == true)
            {
              ret = xelsReadHeader(p_xel);
            }
          }

          if(ret == true)
          {
            p_xel->status.current = NEW_CONNECTION;
          }
          else
          {
            p_xel->xel_id = 0;
          }

          if(id < _XELS_END_ID)
          {
            id++;
          }
          else
          {
            id = _XELS_START_ID;
          }
        }

        if(checking_tbl_num >= CONNECTED_XEL_MAX)
        {
          checking_tbl_num = 0;
        }
      }
    }

    bool flag_scan_just_init_time_;
    uint32_t interval_ms_;
};

} // namespace XelNetwork

#endif /* XEL_NET_HPP_ */
