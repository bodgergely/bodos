#include <kernel/sched/sched.h>


pid currpid = 0;


int resched_counter = 0;

void resched(void)
{
	resched_counter++;
	//klog(INFO ,"Inside resched, currpid: %d and resched counter is: %d.\n", currpid, resched_counter);
	ProcEntryTable& procTable = getProcessTable();
	ProcEntry* procOld;
	ProcEntry* procNew;

	procOld = procTable.procEntry(currpid);

	if(procOld->getStatus() == PR_CURR)
	{
		if(procOld->getPriority() > procTable.highestReadyPrio())
		{
			return;
		}
		procOld->setStatus(PR_RUNNABLE);
		procTable.insertToReadyList(currpid);
	}


	currpid = procTable.scheduleNextTask();
	procNew = procTable.procEntry(currpid);
	procNew->setStatus(PR_CURR);

	ctxswitch(&(procOld->_sp), &(procNew->_sp));
	return;
}