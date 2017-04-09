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
protected:
	int  _i;
	int  _j;
	char _c[FOO_PADDING];
};


class Boo
{
#define BOO_PADDING 32
public:
	Boo() {}
	Boo(int i, int j) : _foo(i,j)
	{
		memset(_f, 'E', BOO_PADDING);
	}
	~Boo()
	{
		memset(_f, 'F', BOO_PADDING);
	}
	void print()
	{
		_foo.print();
		for(int i=0;i<BOO_PADDING;i++)
			kprintf("%c", _f[i]);
		kprintf("\n");
	}
protected:
	Foo _foo;
	char _f[BOO_PADDING];

};

// tests
class MemoryTester
{
#define SIZE 100
public:
	MemoryTester() : _allocCount(0), _iters(0)
	{
		memset(_allocs, 0 , sizeof(void*)*SIZE);
	}
	bool run()
	{
		test();
	}
private:
	void test()
	{
		for(int i=0;i<40;i++)
		{
			int likelihood = rand(10) + 1;
			_iters++;
			if(likelihood > 5)
			{
				allocate(1, 3, 34);
			}
			else
			{
				free(1);
			}
		}

	}

	void allocate(int count, int minSize, int maxSize)
	{
		int diff = maxSize - minSize;
		for(int i=0;i<count;i++)
		{
			if(_allocCount >= SIZE)
				return;
			int numBytes = minSize + rand(diff);
			void* p = kmalloc(numBytes);
			if(!p)
			{
				kprintf("Allocation failed, kmalloc returned zero! Looping to save your ass...\n");
				kprintf("iters: %d\n", _iters);
				while(1);
			}
			_allocCount++;
			for(int i=0;i<SIZE;i++)
			{
				if(!_allocs[i])
				{
					_allocs[i] = p;
					break;
				}
			}

		}
	}

	void free(int count)
	{

		for(int i=0;i<count;i++)
		{
			if(_allocCount==0)
				return;
			while(true)
			{
				int idx = rand(SIZE);
				if(_allocs[idx])
				{
					kfree(_allocs[idx]);
					break;
				}
			}
			_allocCount--;
		}

	}


private:
	void* _allocs[SIZE];
	int   _allocCount;
	int	  _iters;
};


}



#endif /* SRC_KERNEL_INCLUDE_KERNEL_MEM_MEMTESTER_H_ */
