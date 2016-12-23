#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <kernel/header.h>
#include "io.h"

void serial_configure_baud_rate(unsigned short com, unsigned short divisor);

/** serial_configure_line:
     *  Configures the line of the given serial port. The port is set to have a
     *  data length of 8 bits, no parity bits, one stop bit and break control
     *  disabled.
     *
     *  @param com  The serial port to configure
     */
void serial_configure_line(unsigned short com);

/** serial_is_transmit_fifo_empty:
     *  Checks whether the transmit FIFO queue is empty or not for the given COM
     *  port.
     *
     *  @param  com The COM port
     *  @return 0 if the transmit FIFO queue is not empty
     *          1 if the transmit FIFO queue is empty
     */
int serial_is_transmit_fifo_empty(unsigned int com);

void serial_write_to_port(const char* data, size_t size, unsigned int com);
int serial_write(const char* data, size_t size);

#endif
