/*
 * sched.h
 *
 *  Created on: Apr 15, 2017
 *      Author: geri
 */

#ifndef SRC_KERNEL_INCLUDE_KERNEL_SCHED_SCHED_H_
#define SRC_KERNEL_INCLUDE_KERNEL_SCHED_SCHED_H_

#include "ctxswitch.h"
#include <kernel/proc/proc.h>
#include <kernel/timer.h>

namespace scheduler
{
void init();
}

void resched(void);

#endif /* SRC_KERNEL_INCLUDE_KERNEL_SCHED_SCHED_H_ */
