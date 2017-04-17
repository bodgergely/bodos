/*
 * Tester.h
 *
 *  Created on: Apr 17, 2017
 *      Author: geri
 */

#ifndef SRC_KERNEL_INCLUDE_KERNEL_TESTS_TESTER_H_
#define SRC_KERNEL_INCLUDE_KERNEL_TESTS_TESTER_H_

#include <kernel/klog.h>

class Tester
{
public:
	Tester() {}
	virtual ~Tester() {}
	virtual void run() = 0;
};



#endif /* SRC_KERNEL_INCLUDE_KERNEL_TESTS_TESTER_H_ */
