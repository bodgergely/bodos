#include <kernel/sched/sched.h>


pid currpid = 0;


void resched(void)
{
	klog(INFO ,"Inside resched, currpid: %d.\n", currpid);
	ProcEntryTable procTable = getProcessTable();
	ProcEntry* procOld;
	ProcEntry* procNew;

	procOld = &(procTable.procEntry(currpid));

	//debug
	procTable._readyList.print();

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
	//debug
	procTable._readyList.print();
	procNew = &(procTable.procEntry(currpid));
	klog(INFO, "prodOld: %d, procNew: %d\n", procOld, procNew);
	procNew->setStatus(PR_CURR);
	klog(INFO, "old task: sp: %d, code: %d and next task to run: pid: %d new proc sp: %d, new proc code: %d", procOld->_sp, procOld->_code, currpid, procNew->_sp, procNew->_code);

	ctxswitch(&procOld->_sp, &procNew->_sp);

	return;
}
