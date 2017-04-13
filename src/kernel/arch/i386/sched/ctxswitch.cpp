/*
 * ctxswitch.cpp
 *
 *  Created on: Apr 11, 2017
 *      Author: geri
 */

#include "ctxswitch.h"

namespace scheduler
{


class Context
{
public:
	Context() : _stack(NULL) {}
	Context(void* stack, const regs& registers) : _stack(stack), _regs(registers)
	{
	}
private:
	void* _stack;
	regs  _regs;
};


class Process
{
public:
	Process(void* code, int id, Context context) : _code(code) ,_id(id), _context(context)
	{
		klog(INFO, "New process created.\n");
	}
private:
	void*   _code;
	int 	_id;
	Context _context;
};


static void foo()
{
	unsigned int i = 0;
	while(true)
	{
		if(!(i % 10000))
			klog(INFO, "Inside ctxswitch::foo() i: %d\n", i);
		i++;
	}
}

inline static void contextSwitch(void* stack, void* foo)
{
	ctxswitch(stack, foo);
}


static void contextSwitchObserver(struct regs* regs)
{
	/* we should know now which Context(Process) we are in right now since we have to save the state (registers etc) so that we can
	 * later restore when switching back to this Context(Process)
	 */
	klog(INFO, "Inside context switch observer.\n");

	int pages = 8;
	void* stack = kmalloc(8 * PAGE_SIZE);
	// we should save the regs
	contextSwitch(stack, (void*)foo);
}


void init()
{
	getTimer().registerObserver(contextSwitchObserver);
}



}




