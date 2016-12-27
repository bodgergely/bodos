
#include <kernel/debug.h>

uint32_t mem_printable(void* mem)
{
	uint32_t res = 0x00000000;
	char v = *(char*)mem;
	res = res | (v & 0x000000ff);
	return res;
}
