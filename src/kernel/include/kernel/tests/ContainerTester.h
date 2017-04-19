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

		insertTest();
		eraseTest();
		klog(INFO, "Queue test okay\n");
	}
private:
	void eraseTest()
	{
		queue<int> q;
		q.insert(5);
		q.insert(5);
		q.insert(5);
		q.erase(5);
		assert_eq(0, q.size());
	}
	void insertTest()
	{
		queue<int> queue;
		queue.insert(1);		// 1
		queue.insert(2);		// 1 2
		assert_eq(1, queue.top());
		queue.insert(0);		// 1 2 0
		int v = queue.dequeue(); // 2 0
		assert_eq(1, v);
		assert_eq(2, queue.size());
		assert_eq(2, queue.top());
		//queue.print();
		queue.insert(1);		// 2 0 1
		v = queue.dequeue();   // 0 1
		queue.insert(v);		// 0 1 2
		assert_eq(2, v);
		v = queue.dequeue();		// 1 2
		assert_eq(1, queue.top());
		assert_eq(2, queue.size());
		queue.insert(v);			// 1 2 0
		assert_eq(3, queue.size());
		assert_eq(1, queue.top());
	}

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
