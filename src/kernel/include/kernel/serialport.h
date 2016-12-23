#include <kernel/header.h>

void serial_write_to_port(const char* data, size_t size, unsigned int com);
int serial_write(const char* data, size_t size);
