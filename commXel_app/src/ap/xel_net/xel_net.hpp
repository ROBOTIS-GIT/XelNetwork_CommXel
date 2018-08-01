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

namespace XelNetwork
{

class Core
{



};

class PlugAndPlay
{
  public:
    PlugAndPlay(bool scan_just_init_time)
      : flag_scan_just_init_time_(scan_just_init_time)
    {
    }

  private:
    bool flag_scan_just_init_time_;

    void scanWhenInit(void)
    {

    }
};

} // namespace XelNetwork

#endif /* XEL_NET_HPP_ */
