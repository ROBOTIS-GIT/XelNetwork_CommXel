/*
 * lwip.h
 *
 *  Created on: 2017. 10. 18.
 *      Author: opus
 */

#ifndef DRV_ETH_H_
#define DRV_ETH_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "hw_def.h"
#include "bsp.h"

#ifdef _USE_HW_ETH

#include "lwip/etharp.h"
#include "lwip/err.h"
#include "lwip/netif.h"
#include "lwip/tcpip.h"
#include "lwip/tcp.h"
#include "lwip/init.h"
#include "lwip/api.h"
#include "lwip/sockets.h"
#include "lwip/dhcp.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "lwipopts.h"

#ifdef _USE_HW_RTOS
#include "rtos.h"
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
err_t lwipHwInit(struct netif *netif);

#endif

#ifdef __cplusplus
}
#endif



#endif /* DRV_ETH_H_ */
