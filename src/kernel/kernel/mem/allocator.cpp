#include <kernel/mem/allocator.h>

namespace memory
{

Header* jump(Header* from, int steps)
{
	Header* curr = from;
	while(curr && steps)
	{
		curr = (Header*)((char*)curr + sizeof(Header) + curr->size);
		steps--;
	}
	return curr;
}

Block::Block() : _start(NULL), _chunckList(NULL), _blockSize(0)
	{
	}
	Block::Block(void* addr, size_t size) : _start(addr), _chunckList(addr), _blockSize(size)
	{
		putHeader(addr, Header(size-sizeof(Header), NULL, NULL, false));
	}
	void Block::init(void* addr, size_t size)
	{
		_start = addr;
		_chunckList = addr;
		_blockSize = size;
		putHeader(addr, Header(size-sizeof(Header), NULL, NULL, false));
	}
	void* Block::allocate(size_t numBytes)
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

		//klog(INFO, "Requested size: %d and iterating through the chunk...\n", numBytes);
		while( (chunk && chunk->taken) || (chunk && chunk->size < numBytes))
		{
			if(chunk->taken != 1 && chunk->taken != 0)
			{
				klog(FATAL, "Heap corruption! chunk: %d and chunk->taken has value: %d and chunk->size: %d\n", chunk, chunk->taken, chunk->size);
				while(1);
			}
			//kprintf("chunk: %d, taken: %d and chunk size: %d\n", chunk, chunk->taken, chunk->size);
			prev = chunk;
			chunk = (Header*)chunk->next;
            //klog(INFO, "prev chunk: %d and next chunk: %d\n", prev, chunk);
		}
		//klog(INFO, "Found chunk at: %d, size:%d\n", chunk, chunk->size);

		if(!chunk)
			return NULL;


		// decide about a split
		const int minChunkSize = 8;
		Header* first = chunk;
		//klog(INFO, "num bytes requested: %d found place at addr: %d, size: %d, prev: %d, next: %d, taken: %d\n", numBytes, first, first->size, first->prev, first->next, first->taken);
		if(first->size < 0)
		{
			klog(ERR, "size is negative!!! Bug\n");
			while(1);
		}
		if(first->size - numBytes > sizeof(Header) + minChunkSize)
		{
			// second
			Header* second = (Header*)(((char*)first) + sizeof(Header) + numBytes);
			int secondSize = first->size - numBytes - sizeof(Header);
			*second = Header(secondSize, (void*)first, first->next, false);
			// here we need to make the first->next (which will be basically the third in the list to refer back not to the first but to the second!!)
			if(first->next)
			{
				((Header*)first->next)->prev = (void*)second;
			}

			*first = Header(numBytes, (void*)prev, (void*)second, true);
		}
		else
		{
			*first = Header(first->size, (void*)prev, (void*)jump(first, 1), true);
		}


		//klog(INFO, "allocated address: %d\n", (void*) ((char*)first + sizeof(Header)));
		return (void*) ((char*)first + sizeof(Header));


	}
	void Block::free(void* addr)
	{
		Header* header = (Header*)((char*)addr - sizeof(Header));
		header->taken = false;
		//kprintf("freeing at: %d size: %d\n", header, header->size);

		bool nextFree = isFree((Header*)header->next);
		bool prevFree = isFree((Header*)header->prev);

		if(nextFree || prevFree)
		{
			if(nextFree)
				header = merge(header, (Header*)header->next);
			if(prevFree)
			{
				header = merge((Header*)header->prev, header);
			}

		}

		//kprintf("After freeing at: %d size: %d\n", header, header->size);


	}

	Header* Block::merge(Header* first, Header* second)
	{
		first->size += sizeof(Header) + second->size;
		first->next = second->next;
		//IMPORTANT TO ADD
		if(second->next)
			((Header*)(second->next))->prev = (void*)first;
		// END OF ADD
		return first;
	}

	bool Block::isFree(const Header* header) { return (header && !(header->taken)); }

	void Block::putHeader(void* addr, const Header& header)
	{
		*((Header*)addr) = header;
	}



}


