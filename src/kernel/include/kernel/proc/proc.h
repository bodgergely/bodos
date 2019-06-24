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
#include <kernel/containers/unordered_map.h>
#include <stdarg.h>
#include <string.h>

using namespace bodos;

#define MAX_PROC_NUM 1024
#define MINSTACK (4)*(PAGE_SIZE)

extern void resched();

typedef int pid;

using namespace bodos;

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
	ProcStatus getStatus() const {return _status;}
	void	   setStatus(ProcStatus status) {_status = status;}
	int	   getPriority() const {return _priority;}
	void   setPriority(int prio) { _priority = prio;}

	// release resources
	void release()
	{
		kfree(_stackmem);
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
	bool insert(pid id)
	{
		return _readyList.insert(id);
	}

	bool erase(pid id)
	{
		if(_readyList.erase(id))
		{
			return true;
		}
		else
			return false;
	}

	pid pop()
	{
		pid id =_readyList.dequeue();
		return id;
	}

	pid peek()
	{
		return _readyList.top();
	}

	int count() const {return _readyList.size() }

	void print() const
	{
		_readyList.print();
	}

	int size() const {return _readyList.size();}

private:
	queue<pid>  	_readyList;
	int				_count;
};

class ProcEntryTable
{
public:
	ProcEntryTable();
	pid  		insert(ProcEntry* proc);
	bool 		erase(pid id);
	ProcEntry* 	procEntry(pid id);
	int			highestReadyPrio()
	{
		pid id = _readyList.top().key;
		return id;
	}
	pid 		scheduleNextTask()
	{
		return _readyList.pop();
	}
	int			totalCount() const { return _numOfProcesses; }
	int 		readyCount() const;
	// debug
	void 		printReadyList() const { _readyList.print(); }
private:
	int		   						_numOfProcesses;
	priority_queue<pid, ReadyList>  _processHierarchy;
	unordered_map<pid, ProcEntry*>	_pidToProcEntryMap;
	ProcEntry*  					_processList[MAX_PROC_NUM];
	bool 	   						_taken[MAX_PROC_NUM];

};

/*
class ProcEntryTable
{
public:
	ProcEntryTable();
	pid  insert(const ProcEntry& proc);
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
	int		   _numOfProcesses;
	ReadyList  _readyList;
	ProcEntry  _processList[MAX_PROC_NUM];
	bool 	   _taken[MAX_PROC_NUM];

};
*/


int 			userret(void);
ProcEntry* 		procent(pid pid);
ProcEntryTable& proctable();
pid  			getpid();
ReadyList& 		readylist();

/*
 * Process manipulation interface
 */
pid 	kthread_create(void* code, uint32_t stacksize, int prio, ProcStatus start_status, bool do_resched, char* name, uint32_t nargs, ...);
int 	ready(pid id);
int 	suspend(pid id);
int 	kill(pid id);
bool 	isvalid(pid id);

namespace processes
{
void init();
}









#endif /* SRC_KERNEL_INCLUDE_KERNEL_PROC_PROC_H_ */
