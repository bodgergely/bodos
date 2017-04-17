/*
 * stack.h
 *
 *  Created on: Apr 16, 2017
 *      Author: geri
 */

#ifndef SRC_KERNEL_ARCH_I386_SCHED_STACK_H_
#define SRC_KERNEL_ARCH_I386_SCHED_STACK_H_

#include <stdint.h>


uint32_t* create_stack(uint32_t* stack, void* code);


#endif /* SRC_KERNEL_ARCH_I386_SCHED_STACK_H_ */
