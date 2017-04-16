#ifndef KPRINTF_H
#define KPRINTF_H

#include <string.h>
#include <stdarg.h>
#include <kernel/tty.h>
#include <kernel/serialport.h>

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

extern "C"
{
int kprintf_level(log_level lev, log_target target, int appendLogLevel, const char *format, ...);


#define kprintf_target(target, format, ...) \
		kprintf_level(INFO, target, 0, format, ##__VA_ARGS__)

#define kprintf(format, ...) \
		kprintf_target(FRAME_BUFFER, format, ##__VA_ARGS__)

}



#endif
