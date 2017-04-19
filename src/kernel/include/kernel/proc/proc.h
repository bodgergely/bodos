/*
 * proc.h
 *
 *  Created on: Apr 15, 2017
 *      Author: geri
 */

#ifndef SRC_KERNEL_INCLUDE_KERNEL_PROC_PROC_H_
#define SRC_KERNEL_INCLUDE_KERNEL_PROC_PROC_H_

#include <kernel/klog.h>
#include <kernel/mem/kmalloc.h>
#include <kernel/sched/ctxswitch.h>
#include <kernel/sched/stack.h>
#include <kernel/containers/queue.h>
#include <stdarg.h>
#include <string.h>

#define MAX_PROC_NUM 256
#define MINSTACK (4)*(PAGE_SIZE)

extern void resched();

typedef int pid;


#define PROC_ERR -1
#define PROC_OK 0
#define PROC_NEX 1

enum ProcStatus
{
	PR_CURR,
	PR_RUNNABLE,
	PR_SUSPENDED,
	PR_DEAD,
	PR_NA
};

class ProcEntry
{
public:
	ProcEntry();
	/*ProcEntry& operator=(const ProcEntry& o)
	{
		kprintf("Copy assignment of ProcEntry\n");
		while(1);
	}
	*/
	ProcEntry(void* code, void* sp, void* stackmem, int prio=10, ProcStatus=PR_RUNNABLE);
	void setID(int id) { _id = id; }
	pid getID() {return _id;}
	ProcStatus getStatus() {return _status;}
	void	   setStatus(ProcStatus status) {_status = status;}
	int	   getPriority() {return _priority;}
	void   setPriority(int prio) { _priority = prio;}

	// release resources
	void release()
	{
		delete _stackmem;
		_stackmem = NULL;
	}

	void print() const
	{
		kprintf("Procentry: _id: %d, _code: %d, _sp: %d, _stackmem: %d, _status: %d, _priority: %d\n", _id, _code,
						_sp, _stackmem, _status, _priority);
	}

public:
	void*   _code;
	void*	_sp;
	void*   _stackmem;
private:
	int 	_id;
	ProcStatus _status;
	int		_priority;

};

class ReadyList
{
public:
	void insert(pid id)
	{
		_readyList.insert(id);
		_count++;
	}

	bool erase(pid id)
	{
		if(_readyList.erase(id))
		{
			_count--;
			return true;
		}
		else
			return false;
	}

	pid pop()
	{
		pid p =_readyList.dequeue();
		_count--;
		return p;
	}

	pid peek()
	{
		return _readyList.top();
	}

	int count() const {return _count;}

	void print() const
	{
		_readyList.print();
	}

private:
	queue<pid>  _readyList;
	int			_count;
};

class ProcEntryTable
{
public:
	ProcEntryTable();
	pid  insert(const ProcEntry& proc, bool runnable);
	bool erase(pid id);
	ProcEntry* procEntry(pid id)
	{
		return _processList + id;
	}
	int	highestReadyPrio()
	{
		pid id = _readyList.peek();
		return _processList[id].getPriority();
	}
	pid scheduleNextTask()
	{
		return _readyList.pop();
	}


	ReadyList*	getReadylist() {return &_readyList;}
	int	totalCount() const {return _numOfProcesses;}
	int readyCount() const {return _readyList.count();}

	// debug
	void printReadyList() const { _readyList.print();}

private:
	int		  _numOfProcesses;
	ReadyList  _readyList;
	ProcEntry _processList[MAX_PROC_NUM];
	bool 	  _taken[MAX_PROC_NUM];

};


int userret(void);
ProcEntry* procent(pid pid);
ProcEntryTable& proctable();
pid  gettid();
ReadyList& readylist();

/*
 * Process manipulation interface
 */
pid  createproc(void* code, uint32_t stacksize, int prio, char* name, uint32_t nargs, ...);
int ready(pid id);
int suspend(pid id);
int kill(pid id);
bool isvalid(pid id);

namespace processes
{
void init();
}









#endif /* SRC_KERNEL_INCLUDE_KERNEL_PROC_PROC_H_ */
