/*
 * ContainerTester.h
 *
 *  Created on: Apr 17, 2017
 *      Author: geri
 */

#ifndef SRC_KERNEL_INCLUDE_KERNEL_TESTS_CONTAINERTESTER_H_
#define SRC_KERNEL_INCLUDE_KERNEL_TESTS_CONTAINERTESTER_H_

#include "Tester.h"
#include <kernel/containers/queue.h>
#include <kernel/containers/unordered_map.h>


using namespace bodos;

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

template<class T>
static void assert_neq(const T& left, const T& right)
{
	if(left != right)
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
		for(int i=0;i<200;i++)		// TODO reenable - I am getting memory allocation problems after running this test
			eraseTest();
		klog(INFO, "MaxHeap test okay\n");
		//while(1);
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
				kprintf("priority_queue is not correct! It is not sorted correctly! Val: %d prev: %d\n", k, prev);
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
		eraseTest2();
		findTest();
		klog(INFO, "Queue test okay\n");
	}
private:
	void eraseTest()
	{
		queue<int> q;
		assert_eq(0, q.erase_item(3));
		q.insert(5);
		q.insert(5);
		q.insert(5);
		int count = q.erase_item(5);
		assert_eq(3, count);
		assert_eq(0, q.size());

		q.insert(5);
		q.insert(4);
		q.insert(5);
		q.insert(6);
		q.insert(5);
		count = q.erase_item(5);
		assert_eq(3, count);
		assert_eq(2, q.size());
	}
	void eraseTest2()
	{
		queue<int> q;
		assert_eq(false, q.erase_idx(0));
		assert_eq(0, q.erase_item(5));
		q.insert(4);
		q.insert(5);
		assert_eq(true, q.erase_idx(0));				
		assert_eq(5, q.top());
		assert_eq(true, q.erase_idx(0));
		q.insert(6);q.insert(7);
		assert_eq(false, q.erase_idx(2));
		assert_eq(true, q.erase_idx(1));
		assert_eq(6, q.top());

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

	void findTest()
	{
		queue<pair<int,int>> queue;
		queue.insert(make_pair(3, 10));
		int key = 3;
		auto result = queue.find([key](auto elem)	
					{
						return elem.first == key; 
					});
		assert_neq((decltype(result))NULL, result);
		assert_eq(3, result->first);
		result = queue.find([](auto elem)	
					{
						return elem.first == 2; 
					});
		assert_eq((decltype(result))NULL, result);
		assert_eq(true, queue.erase_idx(0));
		result = queue.find([](auto elem)	
					{
						return elem.first == 2; 
					});
		assert_eq((decltype(result))NULL, result);
	}

};


class UnorderedMapTester : public Tester
{
public:
	virtual void run()
	{
		insertEraseTest();
		klog(INFO, "UnorderedMap test okay\n");
	}
private:
	void insertEraseTest()
	{
		unordered_map<int, int> map;
		assert_eq(0, (int)map.size());
		map.insert(10, 123);
		map.insert(4, 50);
		assert_eq(2, (int)map.size());
		assert_eq((int*)nullptr, map.find(11));
		assert_eq(50, *map.find(4));
		assert_eq(123, *map.find(10));
		assert_eq(false, map.insert(10, 149));
		assert_eq(true, map.erase(10));
		assert_eq(1, (int)map.size());
		assert_eq((int*)nullptr, map.find(10));
		assert_eq(true, map.erase(4));
		assert_eq(0, (int)map.size());
	}

};

class ContainerTester : public Tester
{
public:
	virtual void run()
	{
		_heapTest.run();
		_queueTest.run();
		_unorderedMapTest.run();
	}
private:
	MaxHeapTester 		_heapTest;
	QueueTester 		_queueTest;
	UnorderedMapTester	_unorderedMapTest;
};


#endif /* SRC_KERNEL_INCLUDE_KERNEL_TESTS_CONTAINERTESTER_H_ */
