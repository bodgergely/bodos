struct cpu_state{
	unsigned int eax;
	unsigned int ecx;
	unsigned int edx;
	unsigned int ebx;
	unsigned int esp;
	unsigned int ebp;
	unsigned int esi;
	unsigned int edi;
} __attribute__((packed));


void interrupt_handler(struct cpu_state cpu, unsigned int error_code, unsigned int interrupt);
void load_idt(void* idt_address);
