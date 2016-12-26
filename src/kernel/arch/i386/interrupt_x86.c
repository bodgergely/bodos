#include <kernel/kprintf.h>
#include "interrupt_x86.h"


void interrupt_handler(struct cpu_state cpu, unsigned int error_code, unsigned int interrupt)
{
	kprintf(INFO, FRAME_BUFFER, "Interrupt happened!\n");
}
