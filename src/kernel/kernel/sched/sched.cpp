#include <kernel/sched/sched.h>




namespace scheduler
{

static void timerObserver(struct regs* regs)
{
	//kprintf("Timer interrupt\n");
	//resched();
	//kprintf("After resched\n");
}

void init()
{
	getTimer().registerObserver(timerObserver);
}

}

extern pid currpid;
int resched_counter = 0;

void resched(void)
{
	irqmask imask = disable();		// DISABLE INTERRUPTS
	resched_counter++;
	//klog(INFO ,"Inside resched, currpid: %d and resched counter is: %d.\n", currpid, resched_counter);
	ProcEntryTable& procTable = proctable();
	ProcEntry* procOld;
	ProcEntry* procNew;

	pid oldpid = currpid;
	procOld = procTable.procEntry(currpid);

	if(procOld->getStatus() == PR_CURR)
	{
		if(procTable.readyCount() == 0 || procOld->getPriority() > procTable.highestReadyPrio())
		{
			return;
		}

		procOld->setStatus(PR_RUNNABLE);
	}

	currpid = procTable.scheduleNextTask();

	if(procOld->getStatus() == PR_RUNNABLE)
		readylist().insert(oldpid, procOld->getPriority());

	procNew = procTable.procEntry(currpid);
	procNew->setStatus(PR_CURR);
	restore(imask);					// ENABLE (RESTORE) INTERRUPTS
	ctxswitch(&(procOld->_sp), &(procNew->_sp));
	return;
}
