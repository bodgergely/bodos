#ifndef TIMER_H
#define TIMER_H

#include <kernel/klog.h>
#include "system.h"
#include "irq.h"

#define TIMER_OBSERVER_MAX 5


class Timer
{
public:
	//typedef void(TimerHandler)(struct regs* regs);
	using TimerHandler = void(struct regs* regs);
	Timer();
	void timer_install();
	bool registerObserver(TimerHandler handler);
	TimerHandler** getObservers() {return _observers;}
private:
	void timer_wait(unsigned int ticks);
private:
	TimerHandler* _observers[TIMER_OBSERVER_MAX];
	int    		  _observerCount{0};
};


Timer& getTimer();



#endif
