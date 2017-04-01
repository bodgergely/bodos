#pragma once
#include "paging.h"
#include <kernel/header.h>
#include <string.h>


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
	Header(Header* next_, int size_, int taken_) : next(next_), size(size_), taken(taken_) {}
	Header* next;
	int size;
	int taken;
};


/*
 * array of pages where each page is responsible of holding a specific size of chunk
 */
class PageArray
{
public:
	PageArray() : _addr(NULL), _count(0), _size(0) {}
	PageArray(void* addr_, int pagecount_) : _addr(addr_), _count(pagecount_), _size(pagecount_*PAGE_SIZE)
	{
		*(Header*)_addr = Header(NULL,0,FALSE);
	}
	Header* header() {return 	(Header*)_addr;}
	void*   address() {return _addr;}
	int 	count() {return 	_count;}
	int 	size() 	{return  	_size;}
private:
	void* 	   _addr;
	int        _count;
	int		   _size;
};

template<unsigned int N>
class PageArrayList
{
public:
	PageArrayList() : _idx(0), _nextFreeSlot(0) {}
	PageArray* head()
	{
		_idx = 0;
		return &_arrays[_idx++];
	}
	PageArray* next()
	{
		if(_idx+1 < N && _arrays[_idx+1].address()!=NULL)
		{
			return &_arrays[_idx++];
		}
		else
			return NULL;
	}

	bool append(const PageArray& pa)
	{
		if(_nextFreeSlot < N)
		{
			_arrays[_nextFreeSlot] = pa;
			return true;
		}
		else
			return false;
	}


private:
	PageArray _arrays[N];
	int		  _idx;
	int		  _nextFreeSlot;
};


class MemoryPool
{
public:
	void  init()
	{
		int numOfPages = 32;
		_pageArrayList.append(PageArray(bulkAllocPages(numOfPages), numOfPages));
	}
	void* allocate(unsigned int bytes)
	{
		PageArray* pageList = _pageArrayList.head();
		Header* header = NULL;
		Header* previousFree = NULL;
		while(true)
		{
			bool found = false;
			header = pageList->header();
			Header* firstFree = NULL;

			if(header->taken)
				header = header->next;
			firstFree = header;

			while(header->size < bytes)
			{
				header = header->next;
				// detect cycle
				if(header == firstFree)
				{
					// try to jump to the next pagelist
					pageList = _pageArrayList.next();
					if(!pageList)
					{
						// we need to bulk allocate
						int pageCount = bytes / PAGE_SIZE + 1;
						if(pageCount < 32) pageCount+=(32-pageCount);
						if(!_pageArrayList.append(PageArray(bulkAllocPages(pageCount), pageCount)))
							return NULL;
					}
				}
				else
					found = true;
			}
			if(found)
				break;
		}

		header->taken = TRUE;
		header->size = bytes;
		// find next free block to link to the current header
		Header* nextHeader = (Header*)((char*)header + sizeof(header) + bytes);
		Header* st = nextHeader;
		bool cycle = false;
		while(nextHeader->taken)
		{
			nextHeader = nextHeader->next;
			// we need to detect cycle
			if(nextHeader == st){
				cycle = true;
				break;
			}
		}
		if(!cycle)
			header->next = nextHeader;
		else
			header->next = header;
		// here we should also delink from the free chain the previous element that points to header since we have become taken the previous->next = header->next
		while(header->next->next)
		{
			if(header->next->next == header)
				previousFree = header->next;
		}

		previousFree->next = header->next;

		return (void*)((char*)header + sizeof(header));

	}
	void  free(void* address) {}
private:
	void* bulkAllocPages(int numOfPages)
	{
		void* mem = alloc_pages(numOfPages);
		memset(mem, 0, numOfPages*PAGE_SIZE);
		return mem;

	}
private:
	PageArrayList<50> _pageArrayList;
};


}


