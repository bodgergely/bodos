#ifndef INTERRUPTS_H
#define INTERRUPTS_H

typedef unsigned long irqmask;

extern "C"
{
void interrupt(int num);
irqmask disable(void);
irqmask restore(irqmask);
}

#endif
