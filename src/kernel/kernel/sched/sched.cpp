#include <kernel/sched/sched.h>




namespace scheduler
{

uint64_t timerInterruptFiredCount = 0;

static void timerObserver(struct regs* regs)
{
	++timerInterruptFiredCount;
	resched();
}

void init()
{
	getTimer().registerObserver(timerObserver);
}

}

extern pid currpid;
uint64_t resched_counter = 0;
uint64_t switched_to_new_process_counter = 0;
/**
 * Make sure to restore the interrupts on all return paths from this function! 
*/
void resched(void)
{
	irqmask imask = disable();		// DISABLE INTERRUPTS
	resched_counter++;
	ProcEntryTable& procTable = proctable();
	ProcEntry* procOld;
	ProcEntry* procNew;

	pid oldpid = currpid;
	procOld = procTable.procEntry(currpid);

	if(procOld->getStatus() == PR_CURR)
	{
		if(procTable.readyCount() == 0 || procOld->getPriority() > procTable.highestReadyPrio())
		{
			restore(imask);					// ENABLE (RESTORE) INTERRUPTS
			return;
		}

		procOld->setStatus(PR_RUNNABLE);
	}

	currpid = procTable.scheduleNextTask();
	if(currpid != oldpid)
		switched_to_new_process_counter++;


	if(procOld->getStatus() == PR_RUNNABLE)
		readylist().insert(oldpid, procOld->getPriority());

	procNew = procTable.procEntry(currpid);
	procNew->setStatus(PR_CURR);
	restore(imask);					// ENABLE (RESTORE) INTERRUPTS
	//klog(INFO, "Switching to: %d\n", currpid);
	ctxswitch(&(procOld->_sp), &(procNew->_sp));
	return;
}
