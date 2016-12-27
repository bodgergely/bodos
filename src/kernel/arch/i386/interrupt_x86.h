
#ifndef INTERRUPT_X86_H
#define INTERRUPT_X86_H

#include <kernel/header.h>

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


void interrupt_handler(struct cpu_state cpu, unsigned int error_code, unsigned int interrupt);
void load_idt(void* idt_address);

typedef void interrupt_handler_signature(struct cpu_state cpu, unsigned int error_code, unsigned int interrupt);

#endif
