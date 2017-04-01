
#ifndef INTERRUPT_X86_H
#define INTERRUPT_X86_H

#include <kernel/header.h>
#include "system.h"

extern "C"
{
void _interrupt_handler(struct regs* regs);


typedef void interrupt_handler_signature(struct regs* regs);

}

#endif
