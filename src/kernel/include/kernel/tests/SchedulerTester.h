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
	kprintf("Inside procedureOne()");
	resched();
}

static void procedureTwo()
{
	kprintf("Inside procedureTwo()");
	resched();
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
		resched();
		klog(INFO, "After creating 2 processes in SchedulerTester\n");
		while(1);
	}
private:

};



#endif /* SRC_KERNEL_INCLUDE_KERNEL_TESTS_SCHEDULERTESTER_H_ */
