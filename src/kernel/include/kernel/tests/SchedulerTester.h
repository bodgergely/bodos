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
	int counter = 0;
	while(true)
	{
		//kprintf("Inside procedureOne(): a: %d b: %d c: %d\n", a, b, c);
		unsigned long long res = longCalculation(1 << 18);
		if(++counter > 20)
			break;
	}
}

static void procedureTwo(int a, int b, int c, int d)
{
	int counter = 0;
	while(true)
	{
		if(++counter > 5)
		{
			//kprintf("procedureTwo will exit.\n");
			break;
		}
		//kprintf("Inside procedureTwo(): a: %d b: %d c: %d d: %d\n", a, b, c, d);
		unsigned long long res = longCalculation(1 << 18);
	}

}


static void small_function(int a, int b, int c)
{
	int counter = 0;
	while(true)
	{
		volatile unsigned long long res = longCalculation(1 << 8);
		if(++counter > 5)
			break;
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
		spawnLots(30);
		simple();
		klog(INFO, "Scheduler test done.\n");
	}
private:
	void spawnLots(int count)
	{
		klog(INFO, "Spawning %d threads of small_function(int a, int b, int c)\n", count);
		for(int i=0;i<count;i++)
		{
			kthread_create((void*)small_function, 4 * PAGE_SIZE, 10, PR_RUNNABLE, false, "ProcedureOne", 3, i, i, i);
		}
		while(true)
		{
			longCalculation(1 << 18);
			if(readylist().size() == 1)
				break;
		}
	}
	void simple()
	{
		klog(INFO, "Simple scheduler test...\n");
		kthread_create((void*)procedureOne, 8 * PAGE_SIZE, 10, PR_RUNNABLE, false, "ProcedureOne", 3, 34, 56, 67);
		kthread_create((void*)procedureTwo, 8 * PAGE_SIZE, 10, PR_RUNNABLE, false, "ProcedureTwo", 4, 89, 435, 3434, 23);
		klog(INFO, "After creating 2 processes in SchedulerTester\n");
		mainProc();
	}
	void mainProc()
	{
		int c = 0;
		bool suspendedPidTwo = false;
		while(true)
		{
			c++;
			//kprintf("Inside mainProc()\n");
			unsigned long long res = longCalculation(1 << 18);
			//ProcEntryTable& procTable = proctable();
			//procTable.printReadyList();
			//while(1);
			//kprintf("Calc res: %d\n", res);
			if(c % 10 == 0){
				if(suspendedPidTwo == false)
				{
					//klog(INFO, "Suspending pid 2\n");
					suspendedPidTwo = true;
					suspend(2);
				}
				else
				{
					//klog(INFO, "Resuming pid 2\n");
					suspendedPidTwo = false;
					ready(2);
				}
			}

			if(c % 10 == 0)
			{
				//klog(INFO, "Trying to resume pid 3.\n");
				int status = ready(3);
				if(status == PROC_NEX)
				{
					//klog(INFO, "Failed to resume pid 3 which is correct since it does not exist!\n");
				}
				else
				{
					klog(ERR, "ERROR - we should not have been able to resume dead process with pid 3!\n");
					while(1);
				}
			}

			if(readylist().size() == 1)
				break;
		}
	}

};



#endif /* SRC_KERNEL_INCLUDE_KERNEL_TESTS_SCHEDULERTESTER_H_ */
