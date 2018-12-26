/*
 * test_vcp.c
 *
 *  Created on: 2018. 2. 13.
 *      Author: kei
 */

#include "hw.h"
#include "test_def.h"

//-- Internal Variables
//


//-- External Variables
//


//-- Internal Functions
//


//-- External Functions
//

#if _TEST_MODBUS
void testModbus(uint32_t baudrate);
#endif

#if _TEST_ETHERNET
void testEthernet(void);
#endif

#if _TEST_USB
void testUSB(void);
#endif

void testMain(void)
{
#if _TEST_USB
  testUSB();
#endif
#if	_TEST_ETHERNET
  testEthernet();
#endif
#if _TEST_MODBUS
  testModbus(57600);
#endif
}


#if _TEST_USB

static uint32_t tx_time_total = 0, tx_length_total = 0;
static uint32_t rx_time_total = 0, rx_length_total = 0;
void testUSB(void)
{
  static uint8_t tx_buffer[1024+512];
  static uint32_t length, pre_time;
  static bool start_flag = false;

#if 0
  length = vcpAvailable();
  while( length > 0 )
  {
    start_flag = true;

    if(length > (uint32_t)sizeof(tx_buffer))
    {
      length = (uint32_t)sizeof(tx_buffer);
    }

    pre_time = micros();
    for(uint32_t i=0; i<length; i++ )
    {
      tx_buffer[i] = vcpRead();
    }
    rx_time_total += (micros() - pre_time);
    rx_length_total += length;

    pre_time = micros();
    vcpWrite(tx_buffer, length);
    for(;;)
    {
      if(CDC_Itf_TxBufLengh() == 0)
      {
        break;
      }
    }
    tx_time_total += (micros() - pre_time);
    tx_length_total += length;

    length = vcpAvailable();
  }

  if(start_flag == true || micros() - pre_time > 1000000)
  {
    pre_time = micros();
    start_flag = false;

    printf("TX : %d/s, RX : %d/s\r\n", (tx_length_total/(tx_time_total/1000000)), (rx_length_total/(rx_time_total/1000000)));
  }
#else

  static uint32_t sec = 0;

//  length = vcpAvailable();
//  while( length > 0 )
//  {
//    if(start_flag == false)
//    {
//      pre_time = millis();
//      start_flag = true;
//    }
//
//    if(length > (uint32_t)2048)
//    {
//      length = 2048;
//    }
//
//    for(uint32_t i=0; i<length; i++ )
//    {
//      vcpRead();
//    }
//
//    rx_length_total += length;
//    length = vcpAvailable();
//  }

  if(vcpIsConnected())
  {
    if(start_flag == false)
    {
      for(uint32_t i = 0; i < sizeof(tx_buffer); i++)
      {
        tx_buffer[i] = i;
      }
    }
    start_flag = true;

    while(1)
    {
      vcpWrite(tx_buffer,sizeof(tx_buffer));
      if(millis() - pre_time >= 1000)
      {
        pre_time = millis();
        sec++;
        //printf("RX : %d/s\r\n", (rx_length_total/sec));
        printf("TX : %d/s\r\n", vcpGetTxCount()/(sec));
      }
    }
  }


#endif
}
#endif

#if _TEST_ETHERNET
#include "ethernet/ap_ethernet.h"
void testEthernet(void)
{
	err_t err;
	socket_t server;
	int conn_sockfd;
	int32_t recv_len;
	uint8_t shared_buf[256];
	u16_t server_port = 50000;

	err = ethernetOpenSocket(&server, server_port, TCP_SERVER);
	if (err == ERR_OK) {
		for (;;) {
			conn_sockfd = ethernetAccept(&server);

			if (conn_sockfd >= 0) {
				ethernetEnableKeepAlive(conn_sockfd);

				recv_len = ethernetRecvTCP(conn_sockfd, shared_buf, sizeof(shared_buf));
				while (recv_len > 0) {
					ethernetSendTCP(conn_sockfd, shared_buf, recv_len);
					recv_len = ethernetRecvTCP(conn_sockfd, shared_buf,
							sizeof(shared_buf));
				}
				ethernetCloseSocket(conn_sockfd); //for passive close (Close wait)
			}

			//delay(1);
		}
		ethernetCloseSocket(server.sockfd);
	}
}
#endif


#if _TEST_MODBUS
#include "modbus/SimpleModbusMaster.h"

void testModbus(uint32_t baudrate)
{
  static bool flag_setup = false;
  static Packet packet;
  static unsigned int regs[4];
  static unsigned int connection_status;
  static char print_buf[256];
  static uint32_t pre_time = 0;

  if(flag_setup == false)
  {
    packet.id = 4;
    packet.function = READ_HOLDING_REGISTERS;//READ_HOLDING_REGISTERS;
    packet.address = 0;
    packet.no_of_registers = 4;
    packet.register_array = regs;

    modbus_configure(baudrate, 1000, 1000, 10, 0, &packet, 1);

    flag_setup = true;
  }

  connection_status = modbus_update(&packet);

  if(connection_status != 1)
  {
    ledOn(0);
    packet.connection = true;

    if(millis() - pre_time > 1000)
    {
      pre_time =  millis();
      sprintf(print_buf, "Connection Fail : %d\r\n", connection_status);
      uartWrite(_DEF_UART3, (uint8_t*)print_buf, (uint32_t)strlen(print_buf));
    }
  }
  else
  {
    ledOff(0);

    if(millis() - pre_time > 1000)
    {
      pre_time =  millis();
      sprintf(print_buf, "\t [RECV DATA] GPIO1:%d, GPIO2:%d, ADC1:%d, ADC2:%d\r\n",
          regs[0], regs[1], regs[2], regs[3]);
      uartWrite(_DEF_UART3, (uint8_t*)print_buf, (uint32_t)strlen(print_buf));
    }
  }
}

#endif /* _TEST_MODBUS */
