#ifndef TTY_H
#define TTY_H


#include <stddef.h>


void terminal_initialize(void);

void terminal_setcolor(uint8_t color);

int terminal_write(const char* data, size_t size);

void terminal_writestring(const char* data);

#endif
