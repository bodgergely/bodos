#include <kernel/proc/proc.h>


ProcEntryTable* 	ptable;

namespace processes
{
void init()
{
	ptable = new ProcEntryTable();
	ptable->insert(ProcEntry(NULL,NULL,NULL,10,PR_CURR), false);		// represents the first boot kernel process and do not put it on the runnable list
}
}

pid currpid = 0;

pid gettid()
{
	return currpid;
}

ProcEntry* procent(pid pid)
{
	return ptable->procEntry(pid);
}


int userret(void)
{
    return kill(gettid());
}

int ready(pid id)
{
	if(!isvalid(id))
		return PROC_NEX;
	ProcEntry* p = procent(id);
	if(p->getStatus() == PR_RUNNABLE)
		return PROC_ERR;
	ptable->_readyList.insert(id);
	return PROC_OK;
}

int suspend(pid id)
{
	//klog(INFO, "suspend(pid id) not implemented yet.\n");
	if(!isvalid(id))
		return PROC_NEX;
	ProcEntry* pe = procent(id);
	if(pre->)
	ptable->_readyList.erase(id);
	procent(id)->setStatus(PR_SUSPENDED);
	return PROC_OK;
}

int kill(pid id)
{
	if(!isvalid(id))
			return PROC_NEX;
	ProcEntry* p = procent(id);
	if(p->getStatus() == PR_RUNNABLE)
		suspend(pid);

	p->setStatus(PR_DEAD);

	p->release();
	resched();
	return PROC_OK;
}

ProcEntryTable& getProcessTable()
{
	return *ptable;
}


ProcEntryTable::ProcEntryTable() : _numOfProcesses(0)
{
	memset((void*)_taken, false, sizeof(_taken));
}


pid ProcEntryTable::insert(const ProcEntry& proc, bool runnable)
{
	int i=0;
	for(;i<MAX_PROC_NUM;i++)
	{
		if(!_taken[i])
			break;
	}
	if(i == MAX_PROC_NUM)
	{
		klog(ERR, "All slots are taken in proc table! Failed to allocate process!\n");
		return -1;
	}

	_processList[i] = proc;
	_processList[i].setID(i);
	_taken[i] = true;
	_numOfProcesses++;

	if(runnable)
	{
		_readyList.insert(i);
	}

	return i;
}

bool ProcEntryTable::erase(pid id)
{
	if(id < MAX_PROC_NUM && _taken[id] == true)
	{
		_taken[id] = false;
		_processList[id].release();
		_numOfProcesses--;
		return true;
	}
	else
		return false;
}


ProcEntry::ProcEntry() : _code(NULL), _sp(NULL), _stackmem(NULL), _id(-1), _status(PR_NA), _priority(10)
{
}


ProcEntry::ProcEntry(void* code, void* sp, void* stackmem, int prio, ProcStatus status) : _code(code), _sp(sp), _stackmem(stackmem), _id(-1), _status(status), _priority(prio)
{
	//klog(INFO, "New process created with code: %d and stack: %d and prio: %d\n", _code, _sp, _priority);
}


