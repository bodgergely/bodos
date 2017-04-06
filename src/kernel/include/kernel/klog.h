#ifndef KLOG_H
#define KLOG_H

#include <kernel/kprintf.h>

#define klog(info, format, ...) kprintf_level(info, FRAME_BUFFER, 1, format, ##__VA_ARGS__)

#endif
