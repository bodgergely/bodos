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


static unsigned long long longCalculation(unsigned long long count)
{
	volatile unsigned long long res = 0;
	volatile unsigned long long acc = 1;
	for(unsigned long long i=0;i<count;i++)
	{
		acc += (acc + 1);
	}
	res = acc;
	return res;
}


static void procedureOne()
{
	while(true)
	{
		kprintf("Inside procedureOne()\n");
		unsigned long long res = longCalculation(1 << 26);
		//kprintf("Calc res: %d\n", res);
		ProcEntryTable& procTable = getProcessTable();
		procTable.printReadyList();
		//while(1);
		//klog(INFO, "procTable: %d and procTable.procEntry(0): %d\n", &procTable, procTable.procEntry(0));
		//procTable.procEntry(0)->print();
		//procTable.procEntry(1)->print();
		//while(1);
		resched();
	}
}

static void procedureTwo()
{
	while(true)
	{
		kprintf("Inside procedureTwo()\n");
		unsigned long long res = longCalculation(1 << 26);
		ProcEntryTable& procTable = getProcessTable();
		procTable.printReadyList();
		//kprintf("Calc res: %d\n", res);
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
		//pid createProcess(void* code, uint32_t stacksize, int prio, char* name, uint32_t nargs, ...)
		createProcess((void*)procedureOne, 8 * PAGE_SIZE, 10, "ProcedureOne", 0);
		createProcess((void*)procedureTwo, 8 * PAGE_SIZE, 10, "ProcedureTwo", 0);
		klog(INFO, "After creating 2 processes in SchedulerTester\n");
		mainProc();
	}
private:
	void mainProc()
	{
		int c = 0;
		while(true)
		{
			kprintf("Inside mainProc()\n");
			unsigned long long res = longCalculation(1 << 26);
			ProcEntryTable& procTable = getProcessTable();
			procTable.printReadyList();
			//while(1);
			//kprintf("Calc res: %d\n", res);
			resched();
		}
	}

};



#endif /* SRC_KERNEL_INCLUDE_KERNEL_TESTS_SCHEDULERTESTER_H_ */
