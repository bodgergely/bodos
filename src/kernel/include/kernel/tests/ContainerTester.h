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

class MaxHeapTester : public Tester
{
public:
	virtual void run()
	{
		insertTest();
		//eraseTest();
		klog(INFO, "MaxHeap test okay\n");
		while(1);
	}
private:
	void insertTest()
	{
		priority_queue<int> pq;
		pq.insert(7);
		pq.insert(4);
		pq.insert(9);
		pq.insert(15);
		pq.insert(2);
		assert_eq(5, pq.size());
		assert_eq(15, pq.top());
	}
};


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
		int count = q.erase(5);
		assert_eq(3, count);
		assert_eq(0, q.size());

		q.insert(5);
		q.insert(4);
		q.insert(5);
		q.insert(6);
		q.insert(5);
		count = q.erase(5);
		assert_eq(3, count);
		assert_eq(2, q.size());
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
		_heap.run();
		_queueTest.run();
	}
private:
	MaxHeapTester _heap;
	QueueTester _queueTest;
};


#endif /* SRC_KERNEL_INCLUDE_KERNEL_TESTS_CONTAINERTESTER_H_ */
