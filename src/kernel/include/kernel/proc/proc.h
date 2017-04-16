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

extern void resched();

typedef int pid;

enum ProcStatus
{
	PR_CURR,
	PR_RUNNABLE,
	PR_SUSPENDED
};

class ProcEntry
{
public:
	ProcEntry();
	ProcEntry(void* code, void* sp, int prio=10, ProcStatus=PR_RUNNABLE);
	void setID(int id) { _id = id; }
	pid getID() {return _id;}
	ProcStatus getStatus() {return _status;}
	void	   setStatus(ProcStatus status) {_status = status;}
	int	   getPriority() {return _priority;}
	void   setPriority(int prio) { _priority = prio;}
public:
	void*   _code;
	void*	_sp;
private:
	int 	_id;
	ProcStatus _status;
	int		_priority;

};



class ProcEntryTable
{
public:
	ProcEntryTable();
	pid  insert(ProcEntry proc);
	bool erase(pid id);
	ProcEntry& procEntry(pid id)
	{
		return _processList[id];
	}
	int	highestReadyPrio()
	{
		pid id = _readyList.top();
		return _processList[id].getPriority();
	}
	void insertToReadyList(pid id)
	{
		_readyList.insert(id);
	}
	pid scheduleNextTask()
	{
		return _readyList.dequeue();
	}
	queue<int>  _readyList;
private:
	int		  _numOfProcesses;
	ProcEntry _processList[MAX_PROC_NUM];
	bool 	  _taken[MAX_PROC_NUM];

};



ProcEntryTable& getProcEntryTable();


#define STACK_SIZE_DEFAULT (8)*(PAGE_SIZE)
pid createProcess(void* code);
ProcEntryTable& getProcessTable();

namespace processes
{
void init();
}









#endif /* SRC_KERNEL_INCLUDE_KERNEL_PROC_PROC_H_ */
