#pragma once
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>

#define PAGE_SIZE 1 << 12

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

		printf("allocate: Size requested: %d, first chunk address: %d\n", numBytes, chunk);

		// debug
		/*Header* dchunk = chunk;
		while(dchunk)
		{
			printf("Chunk address: %d size: %d taken: %d prev: %d next: %d\n", dchunk, dchunk->size, dchunk->taken, dchunk->prev, dchunk->next);
			if(!dchunk->next)
							break;
			dchunk = (Header*) ((void*)dchunk + sizeof(Header) + dchunk->size);
		}
		*/
		//
		char buffer[1024];
		while( (chunk && chunk->taken) || (chunk && chunk->size < numBytes))
		{
			sprintf(buffer, "chunk: %d, taken: %d and chunk size: %d chunk->next: %d", chunk, chunk->taken, chunk->size, chunk->next);
			std::cout << buffer <<std::endl;
			prev = chunk;
			chunk = (Header*)chunk->next;
		}


		if(!chunk)
			return NULL;


		// decide about a split
		const int minChunkSize = 8;
		Header* first = chunk;

		sprintf(buffer, "num bytes requested: %d found place at addr: %d, size: %d, prev: %d, next: %d, taken: %d\n", numBytes, first, first->size, first->prev, first->next, first->taken);
		std::cout << buffer << std::endl;
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
			*first = Header(first->size, (void*)prev, (void*)jump(first, 1), true);
		}

		printf("allocated address: %d\n", (void*) ((char*)first + sizeof(Header)));
		return (void*) ((char*)first + sizeof(Header));


	}
	void free(void* addr)
	{
		Header* header = (Header*)((char*)addr - sizeof(Header));
		header->taken = false;
		printf("freeing at: %d size: %d\n", header, header->size);

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

		char buffer[1024];
		sprintf(buffer, "After freeing at: %d size: %d\n", header, header->size);
		std::cout << buffer << std::endl;


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
		void* addr = malloc(numOfPages*PAGE_SIZE);
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




Heap heap;

void* kmalloc(size_t bytes)
{
	return heap.allocate(bytes);
}

void kfree(void* addr)
{
	heap.free(addr);
}

#define NUM_PAGES_HEAP 1 << 16

void init()
{
	heap.init(NUM_PAGES_HEAP);
}



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




class Foo
{
#define FOO_PADDING 8
public:
	Foo() : _i(0), _j(0)
	{
		memset(_c, 'C', FOO_PADDING);
	}
	Foo(int i, int j) : _i(i), _j(j)
	{
		memset(_c, 'C', FOO_PADDING);
	}
	~Foo()
	{
		_i = 253;
		_j = 254;
		memset(_c, 'D', FOO_PADDING);
	}
	void print()
	{
		printf("%d", _i);
		printf("%d", _j);
		for(int i=0;i<FOO_PADDING;i++)
			printf("%c", _c[i]);
		//printf("\n");
	}
protected:
	int  _i;
	int  _j;
	char _c[FOO_PADDING];
};


class Boo
{
#define BOO_PADDING 32
public:
	Boo() {}
	Boo(int i, int j) : _foo(i,j)
	{
		memset(_f, 'E', BOO_PADDING);
	}
	~Boo()
	{
		memset(_f, 'F', BOO_PADDING);
	}
	void print()
	{
		_foo.print();
		for(int i=0;i<BOO_PADDING;i++)
			printf("%c", _f[i]);
		printf("\n");
	}
protected:
	Foo _foo;
	char _f[BOO_PADDING];

};

// tests
class MemoryTester
{
#define SIZE 100
public:
	MemoryTester() : _allocCount(0), _iters(0)
	{
		memset(_allocs, 0 , sizeof(void*)*SIZE);
	}
	bool run()
	{
		test();
	}
private:
	void test()
	{
		srand(time(NULL));
		for(int i=0;i<1000;i++)
		{
			int likelihood = rand() % 10 + 1;
			_iters++;
			if(likelihood > 4)
			{
				allocate(1, 3, 34);
			}
			else
			{
				ffree(1);
			}
		}

	}

	void allocate(int count, int minSize, int maxSize)
	{
		int diff = maxSize - minSize;
		for(int i=0;i<count;i++)
		{
			if(_allocCount >= SIZE)
				return;
			int numBytes = minSize + rand() % diff;
			void* p = kmalloc(numBytes);
			if(!p)
			{
				printf("Allocation failed, kmalloc returned zero! Looping to save your ass...\n");
				printf("iters: %d\n", _iters);
				while(1);
			}
			_allocCount++;
			for(int i=0;i<SIZE;i++)
			{
				if(!_allocs[i])
				{
					_allocs[i] = p;
					break;
				}
			}

		}
	}

	void ffree(int count)
	{

		for(int i=0;i<count;i++)
		{
			if(_allocCount==0)
				return;
			while(true)
			{
				int idx = rand() % SIZE;
				if(_allocs[idx])
				{
					kfree(_allocs[idx]);
					_allocs[idx] = NULL;
					break;
				}
			}
			_allocCount--;
		}

	}


private:
	void* _allocs[SIZE];
	int   _allocCount;
	int	  _iters;
};
