#ifndef KMALLOC_H
#define KMALLOC_H
#include "allocator.h"


void* kmalloc(size_t bytes);
void  kfree(void* addr);



namespace memory
{

void init();


}



#endif
