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

Header* jump(Header* from, int steps);


class Block
{
public:
	Block();
    Block(void* addr, size_t size);
    void init(void* addr, size_t size);
	void* allocate(size_t numBytes);
	void free(void* addr);
private:
	Header* merge(Header* first, Header* second);
    bool isFree(const Header* header); 

	void putHeader(void* addr, const Header& header);
private:
	void*  _start;
	void*  _chunckList;
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
	void free(void* addr)
	{
		return _block.free(addr);
	}
private:
	Block _block;

};


}


