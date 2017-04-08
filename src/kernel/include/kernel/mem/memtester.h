/*
 * memtester.h
 *
 *  Created on: Apr 6, 2017
 *      Author: geri
 */

#ifndef SRC_KERNEL_INCLUDE_KERNEL_MEM_MEMTESTER_H_
#define SRC_KERNEL_INCLUDE_KERNEL_MEM_MEMTESTER_H_

#include <kernel/klog.h>
#include <kernel/mem/kmalloc.h>
#include <string.h>

namespace memory
{


class Foo
{
#define FOO_PADDING 8
public:
	Foo() : _i(0), _j(0)
	{
		memset(_c, 'C', FOO_PADDING);
	}
	Foo(int i, int j) : _i(i), _j(j)
	{
		memset(_c, 'C', FOO_PADDING);
	}
	~Foo()
	{
		_i = 253;
		_j = 254;
		memset(_c, 'D', FOO_PADDING);
	}
	void print()
	{
		kprintf("%d", _i);
		kprintf("%d", _j);
		for(int i=0;i<FOO_PADDING;i++)
			kprintf("%c", _c[i]);
		//kprintf("\n");
	}
private:
	int  _i;
	int  _j;
	char _c[FOO_PADDING];
};

// tests
class MemTester
{
#define SIZE 3
public:
	MemTester()
	{
		memset(_objectPointers, 0 , sizeof(Foo));
	}
	bool run()
	{
		//performAllocs(1000);
		test();
	}
private:
	void performAllocs(int count)
	{
		for(int i=0;i<count;i++)
		{
			void* mem = kmalloc(10);
			strcpy((char*)mem, "Vivek");
			kprintf((char*)mem);
		}
	}

	void test()
	{
		for(int i=0;i<SIZE;i++)
		{
			_objectPointers[i] = new Foo(4,5);
		}

		print();

		destroy(1);

	}

	void print()
	{
		for(int i=0;i<SIZE;i++)
		{
			_objectPointers[i]->print();
		}
		kprintf("\n-------------------------------\n");
	}

	void destroy(int i)
	{
		kprintf("Destroying element at: %d\n", i);
		delete _objectPointers[i];
		print();

	}

	void destroyAndCreate(int i, int k, int l)
	{
		destroy(i);
		kprintf("Creating element at: %d with values: %d and %d\n", i, k, l);
		_objectPointers[i] = new Foo(k, l);
		_objectPointers[i]->print();
	}

private:
	Foo* _objectPointers[SIZE];

};


}



#endif /* SRC_KERNEL_INCLUDE_KERNEL_MEM_MEMTESTER_H_ */
