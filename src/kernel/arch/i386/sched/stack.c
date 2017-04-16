
#include "stack.h"

#include "../paging.h"
#include <kernel/klog.h>

void create_stack(void* stack, void* code)
{
	*((unsigned int*)stack) = (unsigned int)code;		// return address
	*(((unsigned int*)stack) - 1) = (unsigned int)(((unsigned int*)stack) - 1);
	memset((char*)stack-PAGE_SIZE, 0, PAGE_SIZE - sizeof(unsigned int*) * 2 );

	//klog(INFO, "Stack top is at: %d and code to be put there: %d\n", stack, code);
	//klog(INFO, "val at %d is: %d", stack, *(unsigned int*)stack);
	//klog(INFO, "val at %d is: %d", ((unsigned int*)stack) - 1, *( ((unsigned int*)stack) - 1) );
	//while(1);
}


