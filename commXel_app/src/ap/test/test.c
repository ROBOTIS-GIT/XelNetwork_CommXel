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

void testMain(void)
{
#if _TEST_MODBUS
  testModbus(57600);
#endif
}


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
