/*
 * stack.h
 *
 *  Created on: Apr 16, 2017
 *      Author: geri
 */

#ifndef SRC_KERNEL_INCLUDE_KERNEL_SCHED_STACK_H_
#define SRC_KERNEL_INCLUDE_KERNEL_SCHED_STACK_H_


uint32_t* create_stack(void* code, uint32_t* stack, uint32_t nargs, uint32_t* argstart);



#endif /* SRC_KERNEL_INCLUDE_KERNEL_SCHED_STACK_H_ */
