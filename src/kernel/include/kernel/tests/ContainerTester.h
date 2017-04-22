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
		correctness();
		for(int i=0;i<200;i++)
			eraseTest();
		klog(INFO, "MaxHeap test okay\n");
		while(1);
	}
private:
	void eraseTest()
	{
		priority_queue<int> pq;
		pq.insert(5);
		pq.insert(5);
		pq.insert(4);
		pq.insert(5);
		pq.insert(1);
		pq.insert(2);
		pq.insert(5);
		assert_eq(5, pq.top());
		int ec = pq.erase(5);
		assert_eq(4, ec);
		assert_eq(4, pq.top());

		pq.insert(34);
		assert_eq(34, pq.top());
		pq.insert(34);
		pq.insert(34);
		assert_eq(3, pq.erase(34));

	}

	void correctness()
	{
		priority_queue<unsigned> pq;
		const int val_limit = 4000;
		const int count = 2000;

		for(int i=0;i<count;i++)
		{
			unsigned r = rand(val_limit);
			pq.insert(r);
		}

		//pq.print();
		pq.is_correct();

		unsigned prev = val_limit;
		for(int i=0;i<count;i++)
		{
			unsigned v = pq.dequeue();
			//pq.print();
			pq.is_correct();
			if(v > prev)
			{
				kprintf("Heap is not correct! It is not sorted correctly! Val: %d prev: %d\n", v, prev);
				//pq.print();
				while(1);
			}
			prev = v;
		}
	}

	void sortedPrint(priority_queue<int>& pq)
	{
		int count = pq.size();
		kprintf("Priority Queue sorted: \n");
		for(int i=0;i<count;i++)
		{
			int v = pq.dequeue();
			kprintf("%d ", v);
		}
		kprintf("\n");

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
