#include <kernel/mem/kmalloc.h>

using namespace memory;



MemoryPool mempool;


void* kmalloc(size_t bytes)
{

	return mempool.allocate(bytes);
}


