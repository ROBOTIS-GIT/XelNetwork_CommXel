#include "serial_transport_commxel.h"
#include "profile/transport/serial/serial_transport_internal.h"
#include <fcntl.h>
#include <unistd.h>
#include "hw.h"

bool uxr_init_serial_platform(struct uxrSerialPlatform* platform, int fd,
    uint8_t remote_addr, uint8_t local_addr)
{
  (void) remote_addr;
  (void) local_addr;

  /* Open device */
  uartOpen(platform->channel, platform->baudrate);

  return true;
}

bool uxr_close_serial_platform(struct uxrSerialPlatform* platform)
{
  return (OK == uartClose(platform->channel));
}

size_t uxr_write_serial_data_platform(uxrSerialPlatform* platform, uint8_t* buf,
    size_t len, uint8_t* errcode)
{
  size_t rv = 0;

  ssize_t bytes_written = vcpWrite((void*) buf, (size_t) len);
  if (-1 != bytes_written)
  {
    rv = (size_t) bytes_written;
    *errcode = 0;
  }
  else
  {
    *errcode = 1;
  }
  return rv;
}

size_t uxr_read_serial_data_platform(uxrSerialPlatform* platform, uint8_t* buf,
    size_t len, int timeout, uint8_t* errcode)
{
  size_t rv = 0;

  uint32_t pre_time = millis();

  while (rv <= 0 && (millis() - pre_time < timeout))
  {
    rv = vcpAvailable();
  }

  if (0 < rv)
  {
    for (int i = 0; i < rv; i++)
    {
      buf[i] = vcpGetch();
    }
    *errcode = 0;
  }
  else
  {
    *errcode = 1;
  }

  return rv;
}

