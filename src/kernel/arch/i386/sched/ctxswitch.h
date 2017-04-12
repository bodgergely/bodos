/*
 * ctxswitch.h
 *
 *  Created on: Apr 11, 2017
 *      Author: geri
 */

#ifndef SRC_KERNEL_ARCH_I386_SCHED_CTXSWITCH_H_
#define SRC_KERNEL_ARCH_I386_SCHED_CTXSWITCH_H_


#include "../timer.h"
#include "../paging.h"
#include "switch.h"
#include <kernel/klog.h>
#include <kernel/mem/kmalloc.h>

namespace scheduler
{

void init();

}




#endif /* SRC_KERNEL_ARCH_I386_SCHED_CTXSWITCH_H_ */
