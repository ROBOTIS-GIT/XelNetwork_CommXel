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

#include "ethernet/ap_ethernet.h"
#include "ros2/ros2.hpp"
#include "xels/xels.h"
#include "xel_net/xel_net.hpp"
#include "usb_to_dxl/usb_to_dxl.hpp"

void apInit(void);
void apMain(void);


#endif /* SRC_AP_AP_HPP_ */
