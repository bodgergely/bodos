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
public:
	bool run()
	{
		//performAllocs(1000);
		foo();
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

	void foo()
	{
		const int size = 3;
		Foo* fooPointers[size];

		for(int i=0;i<size;i++)
		{
			fooPointers[i] = new Foo(i+1, i+3);
			fooPointers[i]->print();
		}

		delete fooPointers[2];
		fooPointers[2]->print();


		//delete fooPointers[6];
		//fooPointers[6]->print();
		//delete fooPointers[5];
		//fooPointers[5]->print();

	}


};


}



#endif /* SRC_KERNEL_INCLUDE_KERNEL_MEM_MEMTESTER_H_ */
