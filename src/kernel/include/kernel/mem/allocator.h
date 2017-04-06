#pragma once
#include "paging.h"
#include <kernel/header.h>
#include <string.h>
#include <kernel/kprintf.h>


/*
 * use case: user requests some size of bytes. We can request here pages. We need to have a set of pages from we can allocate the bytes.
 * 1. Find the requested number of bytes and give it back to the user. Finding should be constant time
 *
 * 2. Mark that memory as taken with the number of bytes.
 *
 *Allocation (first fit):
 *		1) Start to walk free list and we find the first free chunk that has enough size we:
 *				i) determine whether we should split the free chunk if too big into 2 parts. First part should be the smallest that can satisfy the user's need and the second is the rest.
 *				ii) update the header information on the first and second chunk (put size into both and also have the first->prev.next == second and second.next == first.next and also update the prev pointers)
 *				iii) mark the _first + sizeof(header) as result for the user
 *
 *Deallocation:
 *		1) _addr - sizeof(header) => contains the header info (size, prev block (free or taken) and next )
 *
 */

namespace memory
{

struct Header
{
	Header(size_t size_, void* prev_, void* next_, bool taken_) : size(size_), prev(prev_), next(next_), taken(taken_) {}
	size_t  size;
	void*   prev;		// previous block which can be either free or taken
	void*	next;		//
	bool    taken;
};

class Block
{
public:
	Block() : _start(NULL), _freeList(NULL), _blockSize(0)
	{
	}
	Block(void* addr, size_t size) : _start(addr), _freeList(addr), _blockSize(size)
	{
		putHeader(addr, Header(size-sizeof(Header), NULL, NULL, false));
	}
	void init(void* addr, size_t size)
	{
		_start = addr;
		_freeList = addr;
		_blockSize = size;
		putHeader(addr, Header(size-sizeof(Header), NULL, NULL, false));
	}
	void* allocate(size_t numBytes)
	{
		// start walking the free list
		Header* prev = NULL;
		Header* free = (Header*)_freeList;
		while(free && free->size < numBytes)
		{
			prev = free;
			free = (Header*)free->next;
		}

		if(!free)
			return NULL;

		// decide about a split
		const int minChunkSize = 8;
		Header* first = free;
		if(first->size - numBytes > sizeof(Header) + minChunkSize)
		{

			// second
			Header* second = (Header*)(((char*)first) + sizeof(Header) + numBytes);
			int secondSize = first->size - numBytes - sizeof(Header);
			*second = Header(secondSize, (void*)first, first->next, false);
			// first
			/*
			first->size = numBytes;
			first->next = (void*)second;
			first->prev = (void*)prev;
			first->taken = true;
			*/
			*first = Header(numBytes, (void*)prev, (void*)second, true);
		}
		else
		{
			*first = Header(numBytes, (void*)prev, first->next, true);
		}


		return (void*) ((char*)first + sizeof(Header));


	}
	void free(void* addr)
	{

	}
private:
	inline void putHeader(void* addr, const Header& header)
	{
		*((Header*)addr) = header;
	}

private:
	void*  _start;
	void*  _freeList;
	size_t _blockSize;
};


class Heap
{
public:
	void init(int numOfPages)
	{
		void* addr = alloc_pages(numOfPages);
		_block.init(addr, numOfPages * PAGE_SIZE);
	}
	void* allocate(size_t bytes)
	{
		return _block.allocate(bytes);
	}
private:
	Block _block;

};


}


