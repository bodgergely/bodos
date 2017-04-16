#include <kernel/sched/sched.h>


pid currpid = 0;


int resched_counter = 0;

void resched(void)
{
	resched_counter++;
	klog(INFO ,"Inside resched, currpid: %d and resched counter is: %d.\n", currpid, resched_counter);
	ProcEntryTable& procTable = getProcessTable();
	ProcEntry* procOld;
	ProcEntry* procNew;

	procOld = procTable.procEntry(currpid);
	klog(INFO, "procOld pid: %d currpid: %d and status: %d\n", procOld->getID(), currpid, procOld->getStatus());

	//debug
	procTable._readyList.print();

	//if(resched_counter > 1)
	//	while(1);

	if(procOld->getStatus() == PR_CURR)
	{
		if(procOld->getPriority() > procTable.highestReadyPrio())
		{
			return;
		}

		klog(INFO, "procOld has status PR_CURR\n");
		procOld->setStatus(PR_RUNNABLE);
		procTable.insertToReadyList(currpid);
		klog(INFO, "queue after inserting %d\n", currpid);
		procTable._readyList.print();
	}


	currpid = procTable.scheduleNextTask();
	klog(INFO, "in resched and will be switching to procid: %d\n", currpid);
	//debug
	procTable._readyList.print();

	procNew = procTable.procEntry(currpid);
	klog(INFO, "procTable: %d and prodOld: %d, procNew: %d\n", &procTable, procOld, procNew);
	procNew->setStatus(PR_CURR);
	klog(INFO, "old task: sp: %d, code: %d and next task to run: pid: %d new proc sp: %d, new proc code: %d\n", procOld->_sp, procOld->_code, currpid, procNew->_sp, procNew->_code);
	klog(INFO, "&(procOld->_sp): %d, &(procNew->_sp): %d\n", &(procOld->_sp), &(procNew->_sp));

	//if(resched_counter > 2)
	//	while(1);

	ctxswitch(&(procOld->_sp), &(procNew->_sp));
	//while(1);

	return;
}
