#include <kernel/header.h>

#define NUM_OF_GDT_ENTRIES 3
#define GDT_ENTRY_SIZE 8




struct gdt
{
	unsigned int address;
	unsigned short size;
} __attribute__((packed));


struct gdt setup_gdt_table();
