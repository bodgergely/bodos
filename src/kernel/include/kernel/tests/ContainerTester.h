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
		//for(int i=0;i<200;i++)		// TODO reenable - I am getting memory allocation problems after running this test
		//	eraseTest();
		klog(INFO, "MaxHeap test okay\n");
	}
private:
	void eraseTest()
	{
		const int val = 56;
		priority_queue<int, int> pq;
		pq.insert(5, 1);
		pq.insert(5, 1);
		pq.insert(4, val);
		pq.insert(5, 1);
		pq.insert(1, val);
		pq.insert(2, val);
		pq.insert(5, 1);
		assert_eq(5, pq.top().key);
		int ec = pq.erase(1);
		assert_eq(4, ec);
		assert_eq(4, pq.top().key);
		assert_eq(3, pq.size());

		pq.insert(34, 4);
		assert_eq(34, pq.top().key);
		pq.insert(34, 4);
		pq.insert(34, 4);
		assert_eq(3, pq.erase(4));

	}

	void correctness()
	{
		priority_queue<unsigned, unsigned> pq;
		const int val_limit = 400;
		const int key_limit = 1000;
		const int count = 2000;

		for(int i=0;i<count;i++)
		{
			unsigned r = rand(key_limit);
			pq.insert(r, rand(val_limit));
		}

		//pq.print();
		pq.is_correct();

		unsigned prev = key_limit;
		for(int i=0;i<count;i++)
		{
			unsigned k = pq.dequeue().key;
			//kprintf("%d ", k);
			//pq.print();
			pq.is_correct();
			if(k > prev)
			{
				kprintf("Heap is not correct! It is not sorted correctly! Val: %d prev: %d\n", k, prev);
				pq.print();
				while(1);
			}
			prev = k;
		}
	}

	void sortedPrint(priority_queue<unsigned, unsigned>& pq)
	{
		int count = pq.size();
		kprintf("Priority Queue sorted: \n");
		for(int i=0;i<count;i++)
		{
			unsigned v = pq.dequeue().key;
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
		//_heap.run();  // TODO reenable - I am getting memory allocation problems after running this test
		_queueTest.run();
	}
private:
	MaxHeapTester _heap;
	QueueTester _queueTest;
};


#endif /* SRC_KERNEL_INCLUDE_KERNEL_TESTS_CONTAINERTESTER_H_ */
