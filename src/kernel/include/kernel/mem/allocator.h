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
	Block() : _start(NULL), _chunckList(NULL), _blockSize(0)
	{
	}
	Block(void* addr, size_t size) : _start(addr), _chunckList(addr), _blockSize(size)
	{
		putHeader(addr, Header(size-sizeof(Header), NULL, NULL, false));
	}
	void init(void* addr, size_t size)
	{
		_start = addr;
		_chunckList = addr;
		_blockSize = size;
		putHeader(addr, Header(size-sizeof(Header), NULL, NULL, false));
	}
	void* allocate(size_t numBytes)
	{
		// start walking the free list
		Header* prev = NULL;
		Header* chunk = (Header*)_chunckList;

		//kprintf("allocate: Size requested: %d, first chunk address: %d\n", numBytes, chunk);

		// debug
		/*Header* dchunk = chunk;
		while(dchunk)
		{
			kprintf("Chunk address: %d size: %d taken: %d prev: %d next: %d\n", dchunk, dchunk->size, dchunk->taken, dchunk->prev, dchunk->next);
			if(!dchunk->next)
							break;
			dchunk = (Header*) ((void*)dchunk + sizeof(Header) + dchunk->size);
		}
		*/
		//

		while( (chunk && chunk->taken) || (chunk && chunk->size < numBytes))
		{
			//kprintf("chunk: %d, taken: %d and chunk size: %d\n", chunk, chunk->taken, chunk->size);
			prev = chunk;
			chunk = (Header*)chunk->next;
		}


		if(!chunk)
			return NULL;


		// decide about a split
		const int minChunkSize = 8;
		Header* first = chunk;
		//klog(INFO, "num bytes requested: %d first addr: %d, size: %d, prev: %d, next: %d, taken: %d\n", numBytes, first, first->size, first->prev, first->next, first->taken);
		if(first->size - numBytes > sizeof(Header) + minChunkSize)
		{

			// second
			Header* second = (Header*)(((char*)first) + sizeof(Header) + numBytes);
			int secondSize = first->size - numBytes - sizeof(Header);
			*second = Header(secondSize, (void*)first, first->next, false);
			*first = Header(numBytes, (void*)prev, (void*)second, true);
		}
		else
		{
			*first = Header(numBytes, (void*)prev, first->next, true);
		}

		//klog(INFO, "allocated address: %d\n", (void*) ((char*)first + sizeof(Header)));
		return (void*) ((char*)first + sizeof(Header));


	}
	void free(void* addr)
	{
		Header* header = (Header*)((char*)addr - sizeof(Header));
		header->taken = false;

		bool nextFree = isFree((Header*)header->next);
		bool prevFree = isFree((Header*)header->prev);

		if(nextFree || prevFree)
		{
			if(nextFree)
				header = merge(header, (Header*)header->next);
			if(prevFree)
			{
				header->next = ((Header*)header->prev)->next;
				header = merge((Header*)header->prev, header);
			}

		}
		else
		{
			// we need to find the next free chunk to link to this newly free one
			Header* s = (Header*)header->next;
			while(s && s->taken)
				s = jump(s, 1);
			// we should have the next free chunk here or the NULL pointer
			header->next = s;
			// we do not need to change the size since we did not merge
		}


	}
private:
	Header* merge(Header* first, Header* second)
	{
		first->size += sizeof(Header) + second->size;
		first->next = second->next;
		return first;
	}

	inline bool isFree(const Header* header) { return (header && !(header->taken)); }

	inline void putHeader(void* addr, const Header& header)
	{
		*((Header*)addr) = header;
	}

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


