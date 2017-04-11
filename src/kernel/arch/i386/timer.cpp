#include "timer.h"

/* This will keep track of how many ticks that the system
*  has been running for */
unsigned long timer_ticks = 0;

Timer timer;

Timer& getTimer()
{
	return timer;
}


Timer::Timer() : _observerCount(0)
{
	for(int i=0;i<TIMER_OBSERVER_MAX;i++)
		_observers[i] = NULL;
}


static void timer_irq_handler(struct regs* regs)
{
	 /* Increment our 'tick count' */
	timer_ticks++;
	//18 ticks in a sec approx
	//klog(INFO, "Inside system timer handler! int_no: %d\n", regs->int_no);
	for(int i=0;i<TIMER_OBSERVER_MAX;i++)
	{
		if(getTimer().getObservers()[i])
		{
			getTimer().getObservers()[i](regs);
		}
	}

}

/* Sets up the system clock by installing the timer handler
*  into IRQ0 */
void Timer::timer_install()
{
    /* Installs 'timer_handler' to IRQ0 */
	Timer::_observerCount = 0;
    irq_install_handler(0, timer_irq_handler);
}

bool Timer::registerObserver(TimerHandler handler)
{
	if(Timer::_observerCount<5)
	{
		_observers[_observerCount++] = handler;
	}
	else
	{
		return false;
	}
}

/* This will continuously loop until the given time has
*  been reached */
void Timer::timer_wait(unsigned int ticks)
{
    unsigned long eticks;

    eticks = timer_ticks + ticks;
    while(timer_ticks < eticks);
}
