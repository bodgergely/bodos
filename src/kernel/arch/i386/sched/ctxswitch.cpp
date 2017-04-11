/*
 * ctxswitch.cpp
 *
 *  Created on: Apr 11, 2017
 *      Author: geri
 */

#include "ctxswitch.h"

namespace scheduler
{


static void contextSwitch(struct regs* regs)
{
	klog(INFO, "Inside context switch subject.\n");
}


void init()
{
	getTimer().registerObserver(contextSwitch);
}



}




