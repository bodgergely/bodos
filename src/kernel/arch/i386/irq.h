#ifndef IRQ_H
#define IRQ_H

#include <kernel/klog.h>
#include "system.h"
#include "io.h"
#include "idt.h"

void irq_install();

/* This installs a custom IRQ handler for the given IRQ */
void irq_install_handler(int irq, void (*handler)(struct regs *r));

/* This clears the handler for a given IRQ */
void irq_uninstall_handler(int irq);


#endif
