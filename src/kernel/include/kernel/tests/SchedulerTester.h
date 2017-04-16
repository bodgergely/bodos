/*
 * SchedulerTester.h
 *
 *  Created on: Apr 15, 2017
 *      Author: geri
 */

#ifndef SRC_KERNEL_INCLUDE_KERNEL_TESTS_SCHEDULERTESTER_H_
#define SRC_KERNEL_INCLUDE_KERNEL_TESTS_SCHEDULERTESTER_H_

#include <kernel/sched/sched.h>
#include <kernel/klog.h>

static void procedureOne()
{
	while(true)
	{
		kprintf("Inside procedureOne()\n");
		ProcEntryTable& procTable = getProcessTable();
		procTable._readyList.print();
		klog(INFO, "procTable: %d and procTable.procEntry(0): %d\n", &procTable, procTable.procEntry(0));
		procTable.procEntry(0)->print();
		procTable.procEntry(1)->print();
		//while(1);
		resched();
	}
}

static void procedureTwo()
{
	while(true)
	{
		kprintf("Inside procedureTwo()\n");
		//while(1);
		resched();
	}
}


class SchedulerTester
{
public:
	SchedulerTester()
	{

	}
	void run()
	{
		createProcess((void*)procedureOne);
		createProcess((void*)procedureTwo);
		//resched();
		klog(INFO, "After creating 2 processes in SchedulerTester\n");
		while(1);
	}
private:

};



#endif /* SRC_KERNEL_INCLUDE_KERNEL_TESTS_SCHEDULERTESTER_H_ */
