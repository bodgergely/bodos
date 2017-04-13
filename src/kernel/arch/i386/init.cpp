#include "init.h"
#include "timer.h"
#include "sched/ctxswitch.h"




//http://wiki.osdev.org/C_PlusPlus
extern "C" void __cxa_pure_virtual()
{
    // Do nothing or print an error message.
	klog(FATAL, "__cxa_pure_virtual() has been called!! Fatal error! Looping forever...\n");
	while(1);
}


static void setup_irq_handlers()
{
	getTimer().timer_install();
	scheduler::init();
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

void dummy()
{
	char buff[256];
	strcpy(buff, "Things still work!");
	klog(INFO, "Message: %s\n", buff);
}

void arch_specific_init()
{
	gdt_install();
	klog(INFO, "GDT has been installed.\n");
	setup_idt();
	setup_irq();
	paging_init();
}
