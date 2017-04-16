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

	klog(INFO, "ProcEntryTable::insert, found pos: %d, runnable: %d\n", i, runnable);
	_processList[i] = proc;
	_processList[i].setID(i);
	_taken[i] = true;
	_numOfProcesses++;

	if(runnable)
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


ProcEntry::ProcEntry() : _code(NULL), _sp(NULL), _stackmem(NULL), _id(-1), _status(PR_SUSPENDED), _priority(10)
{
}


ProcEntry::ProcEntry(void* code, void* sp, void* stackmem, int prio, ProcStatus status) : _code(code), _sp(sp), _stackmem(stackmem), _id(-1), _status(status), _priority(prio)
{
	klog(INFO, "New process created with code: %d and stack: %d and prio: %d\n", _code, _sp, _priority);
}

// TODO this is platform dependent code below!
pid createProcess(void* code)
{
	// TODO stack should 16 byte aligned
	void* stack_memory = kmalloc(STACK_SIZE_DEFAULT + 32);
	const int alignment = 16;
	void* stack_end_aligned = stack_memory;
	unsigned remain =  (unsigned int)stack_memory % alignment;
	if(remain)
		stack_end_aligned = stack_memory + (alignment - remain);

	void* stack_start = stack_end_aligned + STACK_SIZE_DEFAULT;

	const int numOfArgsOnSTACK = 10;		// 11 if we count the return address
	const int registerSIZE = 4;
	void* stack_pointer = stack_start - numOfArgsOnSTACK * registerSIZE;		// we have the return address, ebp, eflags, 8 gen regs saved so the esp should be (need to decrement stack pointer by 10 !! And not 11 by the way)
	klog(INFO, "Create process with code: %d and we allocated for it stack start: %d, stack_pointer: %d, stack memory: %d stack_end_aligned: %d\n", code, stack_start,
							stack_pointer, stack_memory, stack_end_aligned);
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
	klog(INFO, "After stack setup: stack start: %d, val: %d\n stack - 4 where base should be: %d\n stack_pointer: %d and val: %d\n", stack_start, *(unsigned int*)stack_start,
																			*(unsigned int*)(stack_start- 4),
																			stack_pointer, *(unsigned int*)stack_pointer
																			);

	//while(1);
	pid id = getProcessTable().insert(ProcEntry(code, stack_pointer, stack_memory), true);
	resched();
}


