#ifndef KLOG_H
#define KLOG_H

#include <kernel/kprintf.h>

#define klog(info, format, ...) kprintf(info, FRAME_BUFFER, format, ##__VA_ARGS__)

#endif
