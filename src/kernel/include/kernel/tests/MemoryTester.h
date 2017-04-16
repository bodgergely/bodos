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

class Shape
{
public:
	Shape(int i) :_v(i) {}
	virtual ~Shape()
	{
		kprintf("In destructor of Shape\n");
	}
	virtual void print()
	{
		kprintf("Shape\n");
	}
protected:
	int _v;
};

class Triangle : public Shape
{
public:
	Triangle(int i) : Shape(i*2), _t(i) {}
	virtual ~Triangle() {kprintf("In destructor of Triangle\n");}
	virtual void print() {kprintf("Triange: %d shape: %d\n", _t, _v);}
private:
	int _t;
};

// tests
class MemoryTester
{
#define SIZE 100
static const int ITER_COUNT = 100000;
static const int GUARD_COUNT = 700;
static const int GUARD_STRING_SIZE = 90;
public:
	MemoryTester() : _guardAllocCount(0), _allocCount(0), _iters(0)
	{
		memset(_allocs, 0 , sizeof(void*)*SIZE);
		memset(_guards, 0 , sizeof(char*)*GUARD_COUNT);
	}
	bool run()
	{
		//Shape* sh = new Triangle(5);
		//sh->print();
		//delete sh;
		//while(1);
		test();
	}
private:
	void test()
	{
		for(int i=0;i<ITER_COUNT;i++)
		{
			int likelihood = rand(10) + 1;
			_iters++;
			if(likelihood > 3)
			{
				int count = rand(7) + 1;
				allocate(count, 3, 34);
				if(!(_iters % 100))
					maybeAllocGuard(30);
			}
			else
			{
				int count = rand(3) + 1;
				free(1);
				if(!(_iters % 200))
					maybeAllocGuard(20);
			}
		}

		// check the guards are not smashed
		if(!checkGuards())
		{
			klog(ERR, "Guards are smashed! Mem test failed!\n");
			while(1);
		}

	}

	bool checkGuards()
	{
		bool good = true;
		for(int i=0;i<_guardAllocCount;i++)
		{
			char* g = _guards[i];
			int j=0;
			for(;j<GUARD_STRING_SIZE;j++)
			{
				if(g[j]!='P')
					return false;
			}
			if(g[j]!='\0')
			{
				return false;
			}
		}
		return good;
	}

	void maybeAllocGuard(int thresholdPercent)
	{
		if((rand(100) + 1) > thresholdPercent && _guardAllocCount < GUARD_COUNT)
		{
			int idx = _guardAllocCount++;
			_guards[idx] = (char*)kmalloc(GUARD_STRING_SIZE+1);
			char* p = _guards[idx];
			memset(p, 'P', GUARD_STRING_SIZE);
			p[GUARD_STRING_SIZE] = '\0';
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
					_allocs[idx] = NULL;
					break;
				}
			}
			_allocCount--;
		}

	}


private:
	void* _allocs[SIZE];
	char* _guards[GUARD_COUNT];
	int   _guardAllocCount;
	int   _allocCount;
	int	  _iters;
};


}



#endif /* SRC_KERNEL_INCLUDE_KERNEL_MEM_MEMTESTER_H_ */
