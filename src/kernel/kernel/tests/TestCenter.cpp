
/*
 * TestCenter.cpp
 *
 *  Created on: Apr 8, 2017
 *      Author: geri
 */

#include <kernel/tests/TestCenter.h>

void TestCenter::run()
{
	klog(INFO, "Running memory tests...\n");
	memory::MemoryTester memtest;
	memtest.run();

	klog(INFO, "Scheduler tester running...\n");
	SchedulerTester schedTester;
	schedTester.run();

	//generateRandomNumbers(10);
}



