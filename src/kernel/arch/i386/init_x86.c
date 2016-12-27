#include "init_x86.h"
#include "gdt.h"
#include "idt.h"

void arch_specific_init()
{
	install_gdt();
	//kprintf(INFO, FRAME_BUFFER, "g.address: %d and g.size: %d\n", g.address, g.size);
	//kprintf(INFO, FRAME_BUFFER, "gdtr address: %d\n", &g);
	kprintf(INFO, FRAME_BUFFER, "GDT has been loaded.\n");
	uint64_t* idt = setup_idt_table();
	kprintf(INFO, FRAME_BUFFER, "IDT has been set up at: %d.\n", idt);
	load_idt(idt);
}
