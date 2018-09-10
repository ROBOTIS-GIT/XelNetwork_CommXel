/*
 * ap.hpp
 *
 *  Created on: Jul 5, 2018
 *      Author: kei
 */

#ifndef SRC_AP_AP_HPP_
#define SRC_AP_AP_HPP_


#include "hw.h"
#include "ap_def.h"
#include "dxl/dxl.h"
#include "qbuffer.h"
#include "ctable/ctable.h"
#include "dxl_ctable/dxl_ctable.h"
#include "dxl_slave/dxl_slave.h"

#include "ethernet/ap_ethernet.h"
#include "ros2/ros2.hpp"
#include "xels/xels.h"
#include "xel_net/xel_net.hpp"
#include "usb_to_dxl/usb_to_dxl.hpp"



typedef struct
{
  err_code_t      err_code;


  uint16_t        model_number;
  uint8_t         firmware_version;


  dxl_node_t      dxl_slave;
  dxl_node_t      *p_dxl_usb;

  ctable_t        ctable;

  bool            dhcp_enable;
  uint8_t         mac_addr[6];
  char            assigned_ip[16];
  char            remote_ip[16];
  uint16_t        remote_port;

  char            node_name[30];
} ap_t;



extern ap_t *p_ap;


void apInit(void);
void apMain(void);


#endif /* SRC_AP_AP_HPP_ */
