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


}


