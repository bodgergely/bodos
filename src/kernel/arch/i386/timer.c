#include "timer.h"

/* This will keep track of how many ticks that the system
*  has been running for */
unsigned long timer_ticks = 0;

static void timer_irq_handler(struct regs* regs)
{
	 /* Increment our 'tick count' */
	timer_ticks++;
	//18 ticks in a sec approx
	//klog(INFO, "Inside system timer handler! int_no: %d\n", regs->int_no);
}

/* Sets up the system clock by installing the timer handler
*  into IRQ0 */
void timer_install()
{
    /* Installs 'timer_handler' to IRQ0 */
    irq_install_handler(0, timer_irq_handler);
}

/* This will continuously loop until the given time has
*  been reached */
void timer_wait(unsigned int ticks)
{
    unsigned long eticks;

    eticks = timer_ticks + ticks;
    while(timer_ticks < eticks);
}
