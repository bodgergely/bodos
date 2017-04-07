#include <kernel/mem/kmalloc.h>

using namespace memory;


#define NUM_PAGES_HEAP 1 << 16   // 65536 pages which equals around 268MB


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



void operator delete(void * p) // throw()
{
	kfree(p);
}

void operator delete(void* p, long unsigned int size) // throw()
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













