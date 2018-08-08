#include <transport/tcp_transport_commxel.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include "lwip/lwip.h"

/*******************************************************************************
 * Private function declarations.
 *******************************************************************************/
static bool send_tcp_msg(void* instance, const uint8_t* buf, size_t len);
static bool recv_tcp_msg(void* instance, uint8_t** buf, size_t* len, int timeout);
static int get_tcp_error();
static uint16_t read_tcp_data(mrTCPTransport* transport);
static inline void disconnect_tcp(mrTCPTransport* transport);

/*******************************************************************************
 * Private function definitions.
 *******************************************************************************/
bool send_tcp_msg(void* instance, const uint8_t* buf, size_t len)
{
  (void)(instance);
  (void)(buf);
  (void)(len);
  bool rv = true;

  return rv;
}

bool recv_tcp_msg(void* instance, uint8_t** buf, size_t* len, int timeout)
{
  (void)(instance);
  (void)(buf);
  (void)(len);
  (void)(timeout);
  bool rv = false;

  return rv;
}

int get_tcp_error()
{
  return errno;
}

uint16_t read_tcp_data(mrTCPTransport* transport)
{
  (void)(transport);
  uint16_t rv = 0;

  return rv;
}

void disconnect_tcp(mrTCPTransport* transport)
{
  (void)(transport);
}

/*******************************************************************************
 * Public function definitions.
 *******************************************************************************/
bool mr_init_tcp_transport(mrTCPTransport* transport, const char* ip, uint16_t port)
{
  (void)(transport);
  (void)(ip);
  (void)(port);
  bool rv = false;

  return rv;
}

bool mr_close_tcp_transport(mrTCPTransport* transport)
{
  (void)(transport);
  return true;
}
