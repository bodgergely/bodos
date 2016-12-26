#include "init_x86.h"
#include "gdt.h"

void arch_specific_init()
{
	struct gdt g = setup_gdt_table();
	load_gdt(&g);
	kprintf(INFO, FRAME_BUFFER, "GDT has been loaded.\n");
	load_idt();
}
