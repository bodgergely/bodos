#include <kernel/kprintf.h>
#include "interrupt_x86.h"


void interrupt_handler(struct cpu_state cpu, unsigned int interrupt, unsigned int error_code)
{
	kprintf(INFO, FRAME_BUFFER, "Interrupt happened! interrupt: %d error_code: %d\n", interrupt, error_code);
}
