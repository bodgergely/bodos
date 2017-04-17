/*
 * ContainerTester.h
 *
 *  Created on: Apr 17, 2017
 *      Author: geri
 */

#ifndef SRC_KERNEL_INCLUDE_KERNEL_TESTS_CONTAINERTESTER_H_
#define SRC_KERNEL_INCLUDE_KERNEL_TESTS_CONTAINERTESTER_H_

#include "Tester.h"

template<class T>
static void assert_eq(const T& left, const T& right)
{
	if(left == right)
		return;
	else
	{
		klog(ERR, "assert_eq failed in ContainerTester!\n");
		while(1);
	}
}

class QueueTester : public Tester
{
public:
	virtual void run()
	{
		_queue.insert(1);		// 1
		_queue.insert(2);		// 1 2
		assert_eq(1, _queue.top());
		_queue.insert(0);		// 1 2 0
		int v = _queue.dequeue(); // 2 0
		assert_eq(1, v);
		assert_eq(2, _queue.size());
		assert_eq(2, _queue.top());
		//_queue.print();
		_queue.insert(1);		// 2 0 1
		v = _queue.dequeue();   // 0 1
		_queue.insert(v);		// 0 1 2
		assert_eq(2, v);
		v = _queue.dequeue();		// 1 2
		assert_eq(1, _queue.top());
		assert_eq(2, _queue.size());
		_queue.insert(v);			// 1 2 0
		assert_eq(3, _queue.size());
		assert_eq(1, _queue.top());

		klog(INFO, "Queue test okay\n");
	}
private:
	queue<int> _queue;
};


class ContainerTester : public Tester
{
public:
	virtual void run()
	{
		_queueTest.run();
	}
private:
	QueueTester _queueTest;
};


#endif /* SRC_KERNEL_INCLUDE_KERNEL_TESTS_CONTAINERTESTER_H_ */
