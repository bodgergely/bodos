#include "init.h"
#include "gdt.h"
#include "idt.h"
#include "irq.h"


static void setup_irq_handlers()
{
	timer_install();
	keyboard_install();
}

static void setup_irq()
{
	irq_install();
	klog(INFO, "IRQ has been installed.\n");
	setup_irq_handlers();
	__asm__ __volatile__ ("sti");
}

static void setup_idt()
{
	idt_install();
	klog(INFO, "IDT has been installed.\n");
	create_idt_entry(0x80, (uint32_t)interrupt_handler_0, 0x0008, 0x8E);
}

void arch_specific_init()
{
	gdt_install();
	klog(INFO, "GDT has been installed.\n");
	setup_idt();
	setup_irq();
}
