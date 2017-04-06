#include <kernel/mem/kmalloc.h>

using namespace memory;


#define NUM_PAGES_HEAP 1 << 16   // 65536 pages which equals around 268MB

Heap heap;

void* kmalloc(size_t bytes)
{
	return heap.allocate(bytes);
}



namespace memory
{

void init()
{
	heap.init(NUM_PAGES_HEAP);
}




}







