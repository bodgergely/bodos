#ifndef KPRINTF_H
#define KPRINTF_H

#include "serialport.h"
#include "terminal.h"
#include "string.h"
#include <stdarg.h>

typedef enum log_level
{
	TRACE,
	INFO,
	WARN,
	ERR,
	FATAL
}log_level;

typedef enum log_target
{
SERIAL_PORT,
FRAME_BUFFER
}log_target;


int kprintf(log_level lev, log_target target, const char *format, ...);

#endif
