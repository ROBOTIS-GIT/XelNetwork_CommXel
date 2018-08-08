#include <fcntl.h>
#include <transport/serial_transport_opencr.h>
#include <unistd.h>
#include "hw.h"

/*******************************************************************************
 * Static members.
 *******************************************************************************/
static int uart_errno = 0;

/*******************************************************************************
 * Private function declarations.
 *******************************************************************************/
static uint16_t read_uart_data(void* instance, uint8_t* buf, size_t len, int timeout);
static bool send_uart_msg(void* instance, const uint8_t* buf, size_t len);
static bool recv_uart_msg(void* instance, uint8_t** buf, size_t* len, int timeout);

/*******************************************************************************
 * Private function definitions.
 *******************************************************************************/
static uint16_t read_uart_data(void* instance, uint8_t* buf, size_t len, int timeout)
{
    uint16_t rv = 0;
    mrSerialTransport* transport = (mrSerialTransport*)instance;

    uint32_t pre_time = millis();

    while(rv <= 0 && (millis() - pre_time < timeout))
    {
        rv = vcp_is_available();
    }

    if (0 < rv)
    {
      for(int i = 0; i < rv; i++)
      {
        buf[i] = vcp_getch();
      }
    }

    return rv;
}

static bool send_uart_msg(void* instance, const uint8_t* buf, size_t len)
{
    bool rv = false;
    mrSerialTransport* transport = (mrSerialTransport*)instance;

    uint16_t bytes_written = write_serial_msg(&transport->serial_io,
                                              buf,
                                              len,
                                              transport->local_addr,
                                              transport->remote_addr);
    if (0 < bytes_written)
    {
        ssize_t bytes_sent = vcp_write((uint8_t*)transport->serial_io.output.buffer, (uint32_t)bytes_written);
        if (0 < bytes_sent && (uint16_t)bytes_sent == bytes_written)
        {
            rv = true;
        }
    }
    uart_errno = rv ? 0 : -1;

    return rv;
}

static bool recv_uart_msg(void* instance, uint8_t** buf, size_t* len, int timeout)
{
    bool rv = true;
    mrSerialTransport* transport = (mrSerialTransport*)instance;
    uint8_t src_addr;
    uint8_t rmt_addr;
    uint8_t bytes_read = read_serial_msg(&transport->serial_io,
                                         read_uart_data,
                                         instance,
                                         transport->buffer,
                                         sizeof(transport->buffer),
                                         &src_addr,
                                         &rmt_addr,
                                         timeout);
    if (0 < bytes_read && src_addr == transport->remote_addr)
    {
        *len = bytes_read;
        *buf = transport->buffer;
    }
    else
    {
        uart_errno = -1;
        rv = false;
    }

    return rv;
}

static int get_uart_error()
{
    return uart_errno;
}

/*******************************************************************************
 * Public function definitions.
 *******************************************************************************/
bool mr_init_uart_transport(mrSerialTransport* transport, const char* device, uint8_t remote_addr, uint8_t local_addr)
{
    bool rv = false;

    /* Open device */
    vcp_init();
    rv = mr_init_uart_transport_fd(transport, 1, remote_addr, local_addr);
    
    return rv;
}

bool mr_init_uart_transport_fd(mrSerialTransport* transport, int fd, uint8_t remote_addr, uint8_t local_addr)
{
    (void)(fd);
    bool rv = false;

    transport->remote_addr = remote_addr;
    transport->local_addr = local_addr;

    /* Init SerialIO. */
    init_serial_io(&transport->serial_io);

    /* Send init flag. */
    uint8_t flag = MR_FRAMING_END_FLAG;
    ssize_t bytes_written = vcp_write(&flag, 1);
    if (0 < bytes_written && 1 == bytes_written)
    {
        /* Interface setup. */
        transport->comm.instance = (void*)transport;
        transport->comm.send_msg = send_uart_msg;
        transport->comm.recv_msg = recv_uart_msg;
        transport->comm.comm_error = get_uart_error;
        transport->comm.mtu = MR_CONFIG_SERIAL_TRANSPORT_MTU;
        rv = true;
    }

    return rv;
}

bool mr_close_uart_transport(mrSerialTransport* transport)
{
    return true;
}
