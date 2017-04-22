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

	procOld = procTable.procEntry(currpid);

	if(procOld->getStatus() == PR_CURR)
	{
		if(procTable.readyCount() == 0 || procOld->getPriority() > procTable.highestReadyPrio())
		{
			readylist().print();
			while(1);	// TODO implement priority queue for scheduler!!!
			return;
		}

		procOld->setStatus(PR_RUNNABLE);
		readylist().insert(procent_prio(currpid, procOld->getPriority()));
	}


	currpid = procTable.scheduleNextTask();
	procNew = procTable.procEntry(currpid);
	procNew->setStatus(PR_CURR);
	restore(imask);					// ENABLE (RESTORE) INTERRUPTS
	ctxswitch(&(procOld->_sp), &(procNew->_sp));
	return;
}
