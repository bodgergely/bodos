#include "interrupt_handler_x86.h"

#include <kernel/kprintf.h>
#include <kernel/debug.h>
#include "system.h"


void _interrupt_handler(struct regs* regs)
{
	klog(INFO, "Interrupt handler - interrupt num: %d and err code: %d gs: %d ds: %d cs: %d\n",regs->int_no, regs->err_code, regs->gs,regs->ds, regs->cs);
}
