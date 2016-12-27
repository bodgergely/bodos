#ifndef GDT_H
#define GDT_H

#include <kernel/header.h>

#define NUM_OF_GDT_ENTRIES 3
#define GDT_ENTRY_SIZE 8




struct gdtr
{
	uint16_t size;
	uint32_t address;
} __attribute__((packed));


struct gdtr setup_gdt_table();
void setGdt(uint32_t gdt, uint16_t size);
void load_gdt(uint32_t gdtr);

#endif
