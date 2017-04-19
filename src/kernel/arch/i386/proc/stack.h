/*
 * stack.h
 *
 *  Created on: Apr 16, 2017
 *      Author: geri
 */

#ifndef SRC_KERNEL_ARCH_I386_SCHED_STACK_H_
#define SRC_KERNEL_ARCH_I386_SCHED_STACK_H_

#include <stdint.h>

/**
 * @ingroup threads
 *
 * Entered when a thread exits by return.
 */

// defined in proc.h
int userret(void);

#define INITRET userret
uint32_t* create_stack(uint32_t* stack, void* code);


#endif /* SRC_KERNEL_ARCH_I386_SCHED_STACK_H_ */
