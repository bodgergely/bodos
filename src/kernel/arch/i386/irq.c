#include "irq.h"

extern "C"
{
extern void irq_routine_0();
extern void irq_routine_1();
extern void irq_routine_2();
extern void irq_routine_3();
extern void irq_routine_4();
extern void irq_routine_5();
extern void irq_routine_6();
extern void irq_routine_7();
extern void irq_routine_8();
extern void irq_routine_9();
extern void irq_routine_10();
extern void irq_routine_11();
extern void irq_routine_12();
extern void irq_routine_13();
extern void irq_routine_14();
extern void irq_routine_15();
}



/* This array is actually an array of function pointers. We use
*  this to handle custom IRQ handlers for a given IRQ */
irq_handler_type* irq_routines[16] =
{
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

/* This installs a custom IRQ handler for the given IRQ */
void irq_install_handler(int irq, irq_handler_type handler)
{
    irq_routines[irq] = handler;
}

/* This clears the handler for a given IRQ */
void irq_uninstall_handler(int irq)
{
    irq_routines[irq] = 0;
}

/* Normally, IRQs 0 to 7 are mapped to entries 8 to 15. This
*  is a problem in protected mode, because IDT entry 8 is a
*  Double Fault! Without remapping, every time IRQ0 fires,
*  you get a Double Fault Exception, which is NOT actually
*  what's happening. We send commands to the Programmable
*  Interrupt Controller (PICs - also called the 8259's) in
*  order to make IRQ0 to 15 be remapped to IDT entries 32 to
*  47 */
static void irq_remap(void)
{
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);
}

/* We first remap the interrupt controllers, and then we install
*  the appropriate ISRs to the correct entries in the IDT. This
*  is just like installing the exception handlers */
void irq_install()
{
    irq_remap();

    create_idt_entry(32, (unsigned)irq_routine_0, 0x08, 0x8E);
    create_idt_entry(33, (unsigned)irq_routine_1, 0x08, 0x8E);
    create_idt_entry(34, (unsigned)irq_routine_2, 0x08, 0x8E);
    create_idt_entry(35, (unsigned)irq_routine_3, 0x08, 0x8E);
    create_idt_entry(36, (unsigned)irq_routine_4, 0x08, 0x8E);
    create_idt_entry(37, (unsigned)irq_routine_5, 0x08, 0x8E);
	create_idt_entry(38, (unsigned)irq_routine_6, 0x08, 0x8E);
	create_idt_entry(39, (unsigned)irq_routine_7, 0x08, 0x8E);
	create_idt_entry(40, (unsigned)irq_routine_8, 0x08, 0x8E);
	create_idt_entry(41, (unsigned)irq_routine_9, 0x08, 0x8E);
	create_idt_entry(42, (unsigned)irq_routine_10, 0x08, 0x8E);
	create_idt_entry(43, (unsigned)irq_routine_11, 0x08, 0x8E);
	create_idt_entry(44, (unsigned)irq_routine_12, 0x08, 0x8E);
	create_idt_entry(45, (unsigned)irq_routine_13, 0x08, 0x8E);
	create_idt_entry(46, (unsigned)irq_routine_14, 0x08, 0x8E);
    create_idt_entry(47, (unsigned)irq_routine_15, 0x08, 0x8E);
}

/* Each of the IRQ ISRs point to this function.
 * The IRQ Controllers need
*  to be told when you are done servicing them, so you need
*  to send them an "End of Interrupt" command (0x20). There
*  are two 8259 chips: The first exists at 0x20, the second
*  exists at 0xA0. If the second controller (an IRQ from 8 to
*  15) gets an interrupt, you need to acknowledge the
*  interrupt at BOTH controllers, otherwise, you only send
*  an EOI command to the first controller. If you don't send
*  an EOI, you won't raise any more IRQs */
extern "C" void irq_handler(struct regs *r)
{
    /* This is a blank function pointer */
    void (*handler)(struct regs *r);

    // if interrupt time is the timer (int number 32) we should ack before the handler goes off
    if(r->int_no == 32) {
        outb(0x20, 0x20);
    }

    /* Find out if we have a custom handler to run for this
    *  IRQ, and then finally, run it */
    handler = irq_routines[r->int_no - 32];
    //klog(INFO, "Interrupt with number: %d and handler: %d\n", r->int_no - 32, handler);
    if (handler)
    {
        handler(r);
    }

    /* If the IDT entry that was invoked was greater than 40
    *  (meaning IRQ8 - 15), then we need to send an EOI to
    *  the slave controller    IDT entries 32-39 are IRQ0 - IRQ7 and entries 40 - 47 are IRQ8 - IRQ15 */
    if (r->int_no >= 40)
    {
        outb(0xA0, 0x20);
    }

    /* In either case, we need to send an EOI to the master
    *  interrupt controller too */
    // only send ack if not timer interrupt since we have already acked above before the handler
    if(r->int_no != 32) {
        outb(0x20, 0x20);
    }
}

