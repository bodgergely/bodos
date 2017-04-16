/*
 * ctxswitch.h
 *
 *  Created on: Apr 15, 2017
 *      Author: geri
 */

#ifndef SRC_KERNEL_INCLUDE_KERNEL_SCHED_CTXSWITCH_H_
#define SRC_KERNEL_INCLUDE_KERNEL_SCHED_CTXSWITCH_H_


/*
 * Arch specific context switch procedure - see assembly file in arch dir
 */
extern "C"
{
void ctxswitch(void* oldSP, void* newSP);
}


#endif /* SRC_KERNEL_INCLUDE_KERNEL_SCHED_CTXSWITCH_H_ */
