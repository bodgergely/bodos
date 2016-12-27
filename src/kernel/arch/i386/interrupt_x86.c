#include <kernel/kprintf.h>
#include <kernel/debug.h>
#include "system.h"
#include "interrupt_x86.h"


void interrupt_handler(unsigned int interrupt, unsigned int error_code)
{
	kprintf(INFO, FRAME_BUFFER, "Interrupt happened! interrupt: %d error_code: %d\n", mem_printable(&interrupt), mem_printable(&error_code));
}


void _interrupt_handler(struct regs* regs)
{
	kprintf(INFO, FRAME_BUFFER, "Interrupt handler - interrupt num: %d and err code: %d gs: %d ds: %d cs: %d\n",regs->int_no, regs->err_code, regs->gs,regs->ds, regs->cs);
}
