/*
 * dxl_ctable.h
 *
 *  Created on: 2017. 7. 18.
 *      Author: baram
 */

#ifndef DXL_CTABLE_H_
#define DXL_CTABLE_H_



#include "hw.h"


#define DXL_MODEL_NUMBER                        464
#define DXL_INIT_ID                             100
#define DXL_INIT_BAUD                           3    // 1Mbps



#define EEP_ADDR_CHECK_AA                       0
#define EEP_ADDR_CHECK_55                       1
#define EEP_ADDR_ID                             2
#define EEP_ADDR_BAUD                           3

#define EEP_ETHERNET_MAC_ADDR                   4   //length = 6
#define EEP_ETHERNET_DHCP_ENABLE                  10  //length = 1
//#define EEP_STATIC_LOCAL_IP                     11  //length = 16
//#define EEP_STATIC_SUBNET                       27  //length = 16
//#define EEP_STATIC_GATEWAY                      43  //length = 16
//#define EEP_STATIC_DNS_SERVER                   59  //length = 16
#define EEP_ETHERNET_REMOTE_IP                  75  //length = 16
#define EEP_ETHERNET_REMOTE_PORT                91  //length = 2
#define EEP_NODE_NAME                           93  //length = 30

#define P_CONST_MODEL_NUMBER                    0
#define P_CONST_MODEL_INFO                      2
#define P_CONST_FW_VERSION                      6
#define P_EEP_ID                                7
#define P_EEP_DXL_BAUDRATE                      8
#define P_EEP_ETHERNET_MAC_ADDR                 9   //length = 6
#define P_EEP_ETHERNET_DHCP_ENABLE              15  //length = 1
//#define P_EEP_STATIC_LOCAL_IP                   16  //length = 16
//#define P_EEP_STATIC_SUBNET                     32  //length = 16
//#define P_EEP_STATIC_GATEWAY                    48  //length = 16
//#define P_EEP_STATIC_DNS_SERVER                 64  //length = 16
#define P_EEP_ETHERNET_REMOTE_IP                80  //length = 16
#define P_EEP_ETHERNET_REMOTE_PORT              96  //length = 2
#define P_ETHERNET_ASSIGNED_IP                  98  //length = 16
#define P_EEP_NODE_NAME                         114 //length = 30



void dxlCtableInit(void);
void dxlCtableUpdate(void);


#endif
