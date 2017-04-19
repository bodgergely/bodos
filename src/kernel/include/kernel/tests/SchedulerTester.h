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


static void procedureOne(int a, int b, int c)
{
	while(true)
	{
		kprintf("Inside procedureOne(): a: %d b: %d c: %d\n", a, b, c);
		unsigned long long res = longCalculation(1 << 26);
		//kprintf("Calc res: %d\n", res);
		//ProcEntryTable& procTable = proctable();
		//procTable.printReadyList();
		//while(1);
		//klog(INFO, "procTable: %d and procTable.procEntry(0): %d\n", &procTable, procTable.procEntry(0));
		//procTable.procEntry(0)->print();
		//procTable.procEntry(1)->print();
		//while(1);
		resched();
	}
}

static void procedureTwo(int a, int b, int c, int d)
{
	int counter = 0;
	while(true)
	{
		if(++counter > 5)
		{
			kprintf("procedureTwo will exit.\n");
			break;
		}
		kprintf("Inside procedureTwo(): a: %d b: %d c: %d d: %d\n", a, b, c, d);
		unsigned long long res = longCalculation(1 << 26);
		ProcEntryTable& procTable = proctable();
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
		//pid createproc(void* code, uint32_t stacksize, int prio, char* name, uint32_t nargs, ...)
		createproc((void*)procedureOne, 8 * PAGE_SIZE, 10, "ProcedureOne", 3, 34, 56, 67);
		createproc((void*)procedureTwo, 8 * PAGE_SIZE, 10, "ProcedureTwo", 4, 89, 435, 3434, 23);
		klog(INFO, "After creating 2 processes in SchedulerTester\n");
		mainProc();
	}
private:
	void mainProc()
	{
		int c = 0;
		bool suspendedPidOne = false;
		while(true)
		{
			c++;
			kprintf("Inside mainProc()\n");
			unsigned long long res = longCalculation(1 << 26);
			//ProcEntryTable& procTable = proctable();
			//procTable.printReadyList();
			//while(1);
			//kprintf("Calc res: %d\n", res);
			if(c % 10 == 0){
				if(suspendedPidOne == false)
				{
					klog(INFO, "Suspending pid 1\n");
					suspendedPidOne = true;
					suspend(1);
				}
				else
				{
					klog(INFO, "Resuming pid 1\n");
					suspendedPidOne = false;
					ready(1);
				}
			}

			if(c % 10 == 0)
			{
				klog(INFO, "Trying to resume pid 2.\n");
				int status = ready(2);
				if(status == PROC_NEX)
				{
					klog(INFO, "Failed to resume pid 2 which is correct since it does not exist!\n");
				}
				else
				{
					klog(ERR, "ERROR - we should not have been able to resume dead process with pid 2!\n");
				}
			}

			resched();
		}
	}

};



#endif /* SRC_KERNEL_INCLUDE_KERNEL_TESTS_SCHEDULERTESTER_H_ */
