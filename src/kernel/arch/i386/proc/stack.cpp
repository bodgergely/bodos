/*
 * stack.cpp
 *
 *  Created on: Apr 17, 2017
 *      Author: geri
 */

#include "stack.h"

#include "../paging.h"
#include <kernel/klog.h>

/*
ctxswitch:
	push ebp		# push ebp onto stack
	mov	 ebp, esp	# record current sp in ebp
	pushf			# push flags onto the stack		decrements sp by 4 (push lower 16 bits of the EFLAGS reg)   http://www.felixcloutier.com/x86/PUSHF:PUSHFD.html
	pusha			# push general regs on stack	pushes 8 regs (8 * 4 = 32 bytes)	decrements sp by 32			http://www.felixcloutier.com/x86/PUSHA:PUSHAD.html

PUSHA:
Temp ← (ESP);
         Push(EAX);
         Push(ECX);
         Push(EDX);
         Push(EBX);
         Push(Temp);
         Push(EBP);
         Push(ESI);
         Push(EDI);


         http://eli.thegreenplace.net/2011/02/04/where-the-top-of-the-stack-is-on-x86/

*/



uint32_t* create_stack(void* code, uint32_t* stack, uint32_t nargs, uint32_t* argstart)
{
	uint32_t* argcurr = argstart = argstart + nargs - 1;
	for(int i=0;i<nargs;i++)
		*stack-- = *argcurr--;

	*stack-- = (uint32_t)INITRET;

	*(stack - 0) = 		(unsigned int)code;		// return address
	*(stack - 1) = (unsigned int)(((unsigned int*)stack) - 1);		// EBP
	*(stack - 2) = 0; 		// EFLAGS
	*(stack - 3) = 0;    // EAX
	*(stack - 4) = 0;    // ECX
	*(stack - 5) = 0;    // EDX
	*(stack - 6) = 0;    // EBX
	*(stack - 7) = (unsigned int)(((unsigned int*)stack) - 2);    // ESP (saved value, the value of sp right after pushing the eflags with pushf)
	*(stack - 8) = (unsigned int)(((unsigned int*)stack) - 1);    // EBP
	*(stack - 9) = 0;    // ESI
	*(stack - 10) = 0;    // EDI

	return stack - 10;
	//memset((char*)stack-PAGE_SIZE, 0, PAGE_SIZE - sizeof(unsigned int*) * 2 );		// null out the rest (EFLAGS and the 8 general regs)

	//klog(INFO, "Stack top is at: %d and code to be put there: %d\n", stack, code);
	//klog(INFO, "val at %d is: %d", stack, *(unsigned int*)stack);
	//for(int i=0;i<11;i++)
	//	klog(INFO, "stack - %d which is val %d is: %d", i, ((unsigned int*)stack) - i, *( ((unsigned int*)stack) - i) );
	//while(1);
}


