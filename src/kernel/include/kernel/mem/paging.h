
#pragma once
#include <stdint.h>
#include <stddef.h>

#define PAGE_SIZE 4096

void* alloc_pages(size_t count);
void  free_pages(void* start, size_t count);
