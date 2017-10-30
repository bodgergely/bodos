#include <kernel/mem/kmalloc.h>

using namespace memory;


Heap heap;

void* kmalloc(size_t bytes)
{
	return heap.allocate(bytes);
}

void kfree(void* addr)
{
	heap.free(addr);
}


void * operator new(size_t n) // throw( bad_alloc)
{
	return kmalloc(n);
}

void * operator new[] (size_t n) // throw( bad_alloc)
{
	return kmalloc(n);
}



void operator delete(void * p) // throw()
{
	kfree(p);
}

void operator delete(void* p, long unsigned int size) // throw()
{
	kfree(p);
}

void operator delete[](void * p) // throw()
{
	kfree(p);
}




namespace memory
{

void init()
{
	heap.init(NUM_PAGES_HEAP);
}
    

}













