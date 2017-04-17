#ifndef IRQ_H
#define IRQ_H

#include <kernel/klog.h>
#include <kernel/system.h>
#include "io.h"
#include "idt.h"

typedef void (irq_handler_type)(struct regs *r);
void irq_install();
/* This installs a custom IRQ handler for the given IRQ */
void irq_install_handler(int irq, irq_handler_type handler);

/* This clears the handler for a given IRQ */
void irq_uninstall_handler(int irq);


#endif
