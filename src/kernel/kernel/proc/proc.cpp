#include <kernel/proc/proc.h>


ProcEntryTable* 	ptable;

namespace processes
{
void init()
{
	ptable = new ProcEntryTable();
	ptable->insert(ProcEntry(NULL,NULL,10,PR_CURR));

}
}


ProcEntryTable& getProcessTable()
{
	return *ptable;
}


ProcEntryTable::ProcEntryTable() : _numOfProcesses(0)
{
	memset((void*)_taken, false, sizeof(_taken));
}


pid ProcEntryTable::insert(ProcEntry proc)
{
	int i=0;
	for(;i<MAX_PROC_NUM;i++)
	{
		if(!_taken[i])
			break;
	}
	if(i == MAX_PROC_NUM)
		return -1;

	_processList[i] = proc;
	_processList[i].setID(i);
	_taken[i] = true;
	_numOfProcesses++;

	_readyList.insert(i);
	return i;
}

bool ProcEntryTable::erase(pid id)
{
	if(id < MAX_PROC_NUM && _taken[id] == true)
	{
		_taken[id] = false;
		_numOfProcesses--;
		return true;
	}
	else
		return false;
}


ProcEntry::ProcEntry() : _code(NULL), _sp(NULL), _id(-1), _status(PR_SUSPENDED), _priority(10)
{
}


ProcEntry::ProcEntry(void* code, void* sp, int prio, ProcStatus) : _code(code), _sp(sp), _id(-1), _status(PR_RUNNABLE), _priority(prio)
{
	klog(INFO, "New process created with code: %d and stack: %d and prio: %d\n", _code, _sp, _priority);
}


pid createProcess(void* code)
{
	// TODO stack should 16 byte aligned
	void* stack_end = kmalloc(STACK_SIZE_DEFAULT);
	void* stack_start = stack_end + STACK_SIZE_DEFAULT;
	void* stack_pointer = stack_start - 8;
	klog(INFO, "Create process with code: %d and we allocated for it stack start: %d, stack_pointer: %d, stack end: %d\n", code, stack_start, stack_pointer, stack_end);
	/*
	 * here we need to set up a stack for this new process - when resched is called and ctxswitch tries to switch to this new stack we
	 * already need to have the saved general registers and the return address on the stack!
	 * ctxswitch:
		push ebp		# push ebp onto stack
		mov	 ebp, esp	# record current sp in ebp
		pushf			# push flags onto the stack
		pusha			# push general regs on stack

	 we need to have it in assembly I think
	 *
	 */
	create_stack(stack_start, code);
	klog(INFO, "After stack setup: stack start: %d, val: %d\n stack + 4 where base should be: %d\n stack_pointer: %d and val: %d\n", stack_start, *(unsigned int*)stack_start,
																			stack_start - 4, *(unsigned int*)(stack_pointer - 4),
																			stack_pointer, *(unsigned int*)stack_pointer
																			);

	while(1);
	pid id = getProcessTable().insert(ProcEntry(code, stack_start - 8));
	resched();
}


