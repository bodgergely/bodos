
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

*/

void create_stack(void* stack, void* code)
{
	*((unsigned int*)stack) = (unsigned int)code;		// return address
	*(((unsigned int*)stack) - 1) = (unsigned int)(((unsigned int*)stack) - 1);		// EBP
	*(((unsigned int*)stack) - 2) = 0; 		// EFLAGS
	*(((unsigned int*)stack) - 3) = 0xDEADBEEF;    // EAX
	*(((unsigned int*)stack) - 4) = 0xDEADBEEF;    // ECX
	*(((unsigned int*)stack) - 5) = 0xDEADBEEF;    // EDX
	*(((unsigned int*)stack) - 6) = 0xDEADBEEF;    // EBX
	*(((unsigned int*)stack) - 7) = (unsigned int)(((unsigned int*)stack) - 2);    // ESP (saved value, the value of sp right after pushing the eflags with pushf)
	*(((unsigned int*)stack) - 8) = (unsigned int)(((unsigned int*)stack) - 1);    // EBP
	*(((unsigned int*)stack) - 9) = 0xDEADBEEF;    // ESI
	*(((unsigned int*)stack) - 10) = 0xDEADBEEF;    // EDI

	//memset((char*)stack-PAGE_SIZE, 0, PAGE_SIZE - sizeof(unsigned int*) * 2 );		// null out the rest (EFLAGS and the 8 general regs)

	//klog(INFO, "Stack top is at: %d and code to be put there: %d\n", stack, code);
	//klog(INFO, "val at %d is: %d", stack, *(unsigned int*)stack);
	//for(int i=0;i<11;i++)
	//	klog(INFO, "stack - %d which is val %d is: %d", i, ((unsigned int*)stack) - i, *( ((unsigned int*)stack) - i) );
	//while(1);
}


