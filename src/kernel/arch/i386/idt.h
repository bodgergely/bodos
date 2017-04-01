#ifndef IDT_H
#define IDT_H

#include <kernel/header.h>
#include "isr.h"

extern "C"
{
extern interrupt_handler_signature interrupt_handler_0;
}


void idt_install();
void create_idt_entry(size_t index, uint32_t handler, uint16_t selector, uint8_t type_attr);

#endif
