#include <uxr/client/profile/transport/udp/udp_transport_baremetal.h>
#include <transport/udp/udp_transport_internal.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include "hw.h"
#include <lwip/lwip.h>

bool uxr_init_udp_platform(uxrUDPPlatform* platform, const char* ip, uint16_t port)
{
  bool rv = false;

  /* Socket initialization */
  platform->socket_fd = socket(PF_INET, SOCK_DGRAM, 0);
  if (-1 != platform->socket_fd)
  {
    /* timeout socket option */
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 100000; //100ms
    setsockopt(platform->socket_fd, SOL_SOCKET, SO_RCVTIMEO,
        (struct timeval * )&tv, sizeof(struct timeval));

    /* Remote IP setup. */
    struct sockaddr_in temp_addr;
    struct sockaddr *p_remote_addr;
    temp_addr.sin_family = AF_INET;
    temp_addr.sin_port = htons(port);
    temp_addr.sin_addr.s_addr = inet_addr(ip);
    p_remote_addr = (struct sockaddr *) &temp_addr;

    /* Remote address filter. */
    int connected = connect(platform->socket_fd, p_remote_addr,
        sizeof(struct sockaddr));
    rv = (0 == connected);
  }
  return rv;
}

bool uxr_close_udp_platform(uxrUDPPlatform* platform)
{
  return (0 == close(platform->socket_fd));
}

size_t uxr_write_udp_data_platform(uxrUDPPlatform* platform, const uint8_t* buf,
    size_t len, uint8_t* errcode)
{
  size_t rv = 0;
  ssize_t bytes_sent = send(platform->socket_fd, (void* )buf, len, 0);
  if (-1 != bytes_sent)
  {
    rv = (size_t) bytes_sent;
    *errcode = 0;
  }
  else
  {
    *errcode = 1;
  }
  return rv;
}

size_t uxr_read_udp_data_platform(uxrUDPPlatform* platform, uint8_t* buf,
    size_t len, int timeout, uint8_t* errcode)
{
  size_t rv = 0;

  ssize_t bytes_received = recv(platform->socket_fd, (void* )buf, len, 0);
  if (-1 != bytes_received)
  {
    rv = (size_t) bytes_received;
    *errcode = 0;
  }
  else
  {
    *errcode = 1;
  }

  return rv;
}
