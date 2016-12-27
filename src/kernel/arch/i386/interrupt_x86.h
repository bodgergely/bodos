
#ifndef INTERRUPT_X86_H
#define INTERRUPT_X86_H

#include <kernel/header.h>
#include "system.h"

struct cpu_state{
	unsigned int edi;
	unsigned int esi;
	unsigned int ebp;
	unsigned int esp;
	unsigned int ebx;
	unsigned int edx;
	unsigned int ecx;
	unsigned int eax;
} __attribute__((packed));


void interrupt_handler(unsigned int interrupt, unsigned int error_code);
void _interrupt_handler(struct regs* regs);
void load_idt(void* idt_address);

typedef void interrupt_handler_signature(struct regs* regs);

#endif
