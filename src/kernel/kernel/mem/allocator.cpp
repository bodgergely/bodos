#include <kernel/mem/allocator.h>

namespace memory
{

static inline int pagesRequired(unsigned int bytes)
{
	int pageCount = bytes / PAGE_SIZE;
	if(bytes % PAGE_SIZE)
		return ++pageCount;

}

void* MemoryPool::allocate(unsigned int bytes)
{
	int pageCount = pagesRequired(bytes);
	return mem;
}
void MemoryPool::free(void* address)
{

}



void MemoryPool::init()
{
	bulkAllocPages(1000);
}



void MemoryPool::bulkAllocPages(int numOfPages)
{
	void* mem = alloc_pages(numOfPages);

}

}


