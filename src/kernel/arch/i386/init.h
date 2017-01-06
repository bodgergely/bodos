#ifndef INIT_X86_H
#define INIT_X86_H

#include <kernel/init.h>
#include "timer.h"
#include "keyboard.h"
#include "gdt.h"
#include "idt.h"
#include "irq.h"
#include "paging.h"

void arch_specific_init();

#endif
