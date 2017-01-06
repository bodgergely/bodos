#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>
#include <kernel/klog.h>
#include "system.h"

#define VIRTUAL_MEM_OFFSET 0xC0000000
#define NUM_OF_PDE 1024
#define NUM_OF_PTE 1024


struct page_directory
{
	uint32_t entries[NUM_OF_PDE];
}__attribute__((packed));

typedef struct page_directory page_directory_t;

struct page_table
{
	uint32_t entries[NUM_OF_PTE];
}__attribute__((packed));

typedef struct page_table page_table_t;


void paging_init();

inline uint32_t phys_to_virtual(uint32_t phys)
{
	return phys + (uint32_t)VIRTUAL_MEM_OFFSET;
}

#endif
