#ifndef INTERRUPTS_X86_H
#define INTERRUPTS_X86_H

#define ICU1 0x20    /* address of primary interrupt control unit */
#define ICU2 0xA0    /* address of slave interrupt control unit   */

#define OCR1 ICU1     /* Operation Command Register for ICU1      */
#define OCR2 ICU2     /* Operation Command Register for ICU2      */
#define IMR1 (ICU1+1) /* Interrupt Mask Register for ICU1         */
#define IMR2 (ICU2+1) /* Interrupt Mask Register for ICU2         */

#define EOI 0x20     /* end-of-interrupt signal                   */

extern "C"
{
void interrupt(int num);
irqmask enable(void);
irqmask disable(void);
irqmask restore(irqmask);
irqmask getirmask(void);
}
#endif
