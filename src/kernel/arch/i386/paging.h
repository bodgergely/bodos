#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>
#include <kernel/klog.h>
#include "system.h"

#define VIRTUAL_MEM_OFFSET 0xC0000000
#define PHYSICAL_MEM_OFFSET 0x100000
#define NUM_OF_PDE 1024
#define NUM_OF_PTE 1024
#define PAGE_SIZE 4096



void paging_init();

void* alloc_pages(size_t count);
void  free_pages(void* start, size_t count);
int stress_test_page_alloc();


#endif
