#ifndef KMALLOC_H
#define KMALLOC_H
#include "allocator.h"



#define NUM_PAGES_HEAP 1 << 16   // 65536 pages which equals around 268MB


void* kmalloc(size_t bytes);
void  kfree(void* addr);



namespace memory
{

void init();


}



#endif
