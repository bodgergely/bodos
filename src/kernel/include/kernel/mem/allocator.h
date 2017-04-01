#pragma once
#include "paging.h"
#include <kernel/header.h>


/*
 * use case: user requests some size of bytes. We can request here pages. We need to have a set of pages from we can allocate the bytes.
 * 1. Find the requested number of bytes and give it back to the user. Finding should be constant time
 * 		Maybe keep track of free list of memory? Possibly have available a list of chunks of a specific sizes
 * 2. Mark that memory as taken with the number of bytes.
 *
 *
 */

namespace memory
{

struct Header
{
	Header(int next_, int size_, int taken_) : next(next_), size(size_), taken(taken_) {}
	int next;
	int size;
	int taken;
};


/*
 * array of pages where each page is responsible of holding a specific size of chunk
 */
struct PageArray
{
	PageArray() : addr(NULL), count(0) {}
	void* 	   addr;
	int        count;
	int		   size;
};


class MemoryPool
{
public:
	void  init()
	{
		int numOfPages = 32;
		void* mem = bulkAllocPages(numOfPages);
		_p.addr = mem;
		_p.count = numOfPages;
		_p.size = PAGE_SIZE * numOfPages;
		*(Header*)_p.addr = Header(0, 0, FALSE);
	}
	void* allocate(unsigned int bytes)
	{
		void* mem = _memList.head().addr;
		Header* header = (Header*)mem;
		Header* start = header;
		while(header->taken || header->size < bytes)
		{
			header = header->next;
			// detect cycle
			if(header == start)
			{
				// we need to bulk allocate
				int pageCount = bytes / PAGE_SIZE + 1 + 32;
				mem = bulkAllocPages(pageCount);
				_memList.append(mem, pageCount);
			}
		}

		if()

	}
	void  free(void* address);
private:
	void* bulkAllocPages(int numOfPages)
	{
		int numPages = 32;
		return alloc_pages(numOfPages);

	}
private:
	PageArray _p;
};


}


