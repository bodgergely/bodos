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
#define SIZE 10
public:
	MemoryTester() : _createFooCount(0), _createBooCount(0)
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
		void* p1 = kmalloc(34);
		void* p2 = kmalloc(40);
		void* p3 = kmalloc(45);
		//void* p4 = kmalloc(49);

		kfree(p2);
		kmalloc(8);
		kmalloc(4);



	}

	void print()
	{
		for(int i=0;i<_createFooCount;i++)
		{
			kprintf("\nObject at: %d\n", _objectPointers[i]);
			_objectPointers[i]->print();
		}
		kprintf("\n-------------------------------\n");
		for(int i=0;i<_createBooCount;i++)
		{
			kprintf("\nObject at: %d\n", _boo[i]);
			_boo[i]->print();
		}
	}

	void destroyFoo(int i)
	{
		kprintf("Destroying element at index: %d which has pointer: %d\n", i, _objectPointers[i]);
		delete _objectPointers[i];
		print();

	}

	void createBoo(int i, int k, int l)
	{
		_boo[i] = new Boo(k, l);
		_createBooCount++;
		kprintf("Created element at index %d pointer: %d with values: %d and %d\n", i, k, l, _boo[i]);
		//_objectPointers[i]->print();
		print();
	}

private:
	Foo* _objectPointers[SIZE];
	Boo* _boo[SIZE];
	int _createFooCount;
	int _createBooCount;

};


}



#endif /* SRC_KERNEL_INCLUDE_KERNEL_MEM_MEMTESTER_H_ */
