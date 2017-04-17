#ifndef INTERRUPTS_H
#define INTERRUPTS_H

typedef unsigned long irqmask;

extern "C"
{
void interrupt(int num);
irqmask enable(void);
irqmask disable(void);
irqmask restore(irqmask);
irqmask getirmask(void);
}

#endif
