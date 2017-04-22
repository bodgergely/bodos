/*
 * proc.cpp
 *
 *  Created on: Apr 17, 2017
 *      Author: geri
 */

#include <kernel/proc/proc.h>
#include <stdint.h>

static uint32_t* getstack(uint32_t size, uint32_t** stackmem)
{
	const int alignment = 16;
	*stackmem = (uint32_t*)kmalloc(size);
	uint32_t* stacktop = *stackmem + size;
	unsigned remain =  (unsigned int)stacktop % alignment;
	if(remain)
		stacktop = stacktop - (alignment - remain);

	return stacktop;
}

// TODO this is platform dependent code below!
pid kthread_create(void* code, uint32_t stacksize, int prio, ProcStatus start_status, bool do_resched, char* name, uint32_t nargs, ...)
{
	// TODO stack should 16 byte aligned
	if(stacksize < MINSTACK)
		stacksize = MINSTACK;

	uint32_t* stackmem;
	uint32_t* stackstart =  getstack(stacksize, &stackmem);

	//const int numOfArgsOnSTACK = 10;		// 11 if we count the return address
	//void* stack_pointer = stackstart - numOfRegsOnSTACK;		// we have the return address, ebp, eflags, 8 gen regs saved so the esp should be (need to decrement stack pointer by 10 !! And not 11 by the way)
	//klog(INFO, "Create process with code: %d and we allocated for it stack start: %d, stack_pointer: %d, stack memory: %d stack_end_aligned: %d\n", code, stack_start,
	//						stack_pointer, stack_memory, stack_end_aligned);
	/*
	 * here we need to set up a stack for this new process - when resched is called and ctxswitch tries to switch to this new stack we
	 * already need to have the saved general registers and the return address on the stack!
	 * ctxswitch:
		push ebp		# push ebp onto stack
		mov	 ebp, esp	# record current sp in ebp
		pushf			# push flags onto the stack
		pusha			# push general regs on stack

	 we need to have it in assembly I think
	 *
	 */

	uint32_t* argstart = &nargs + 1;
	uint32_t* stackpointer = create_stack(code, stackstart, nargs, argstart);
	//klog(INFO, "After stack setup: stack start: %d, val: %d\n stack - 4 where base should be: %d\n stack_pointer: %d and val: %d\n", stack_start, *(unsigned int*)stack_start,
	//																		*(unsigned int*)(stack_start- 4),
	//																		stack_pointer, *(unsigned int*)stack_pointer
	//																		);

	//while(1);
	pid id = proctable().insert(ProcEntry(code, stackpointer, stackmem, prio, start_status));
	if(do_resched)
		resched();
}


