#include <kernel/header.h>
#include <kernel/kprintf.h>
#include "gdt.h"

struct GDT
{
uint32_t base;
uint32_t limit;
uint8_t  type;
};


struct GDT gdtEntries[NUM_OF_GDT_ENTRIES];
uint8_t gdt[GDT_ENTRY_SIZE*NUM_OF_GDT_ENTRIES];


static void init_gdt_entries()
{
	gdtEntries[0] = (struct GDT){.base=0, .limit=0, .type=0};                     // Selector 0x00 cannot be used
	gdtEntries[1] = (struct GDT){.base=0, .limit=0xffffffff, .type=0x9A};         // Selector 0x08 will be our code
	gdtEntries[2] = (struct GDT){.base=0, .limit=0xffffffff, .type=0x92};         // Selector 0x10 will be our data
	//gdtEntries[3] = (struct GDT){.base=&myTss, .limit=sizeof(myTss), .type=0x89}; // You can use LTR(0x18)
}

/**
 * \param target A pointer to the 8-byte GDT entry
 * \param source An arbitrary structure describing the GDT entry
 */
static void encodeGdtEntry(uint8_t *target, struct GDT source)
{
    // Check the limit to make sure that it can be encoded
    if ( (source.limit > 65536) && ((source.limit & 0xFFF) != 0xFFF) )  {
        kprintf(INFO, SERIAL_PORT, "You can't do that!");
    }
    if (source.limit > 65536) {
        // Adjust granularity if required
        source.limit = source.limit >> 12;
        target[6] = 0xC0;
    } else {
        target[6] = 0x40;
    }

    // Encode the limit
    target[0] = source.limit & 0xFF;
    target[1] = (source.limit >> 8) & 0xFF;
    target[6] |= (source.limit >> 16) & 0xF;

    // Encode the base
    target[2] = source.base & 0xFF;
    target[3] = (source.base >> 8) & 0xFF;
    target[4] = (source.base >> 16) & 0xFF;
    target[7] = (source.base >> 24) & 0xFF;

    // And... Type
    target[5] = source.type;
}

inline static void _setup_gdt_table(uint8_t* table)
{
	init_gdt_entries();
	uint8_t* t = table;
	for(int i=0;i<NUM_OF_GDT_ENTRIES;i++)
	{
		encodeGdtEntry(t, gdtEntries[i]);
		t+=GDT_ENTRY_SIZE;
	}
}

struct gdt setup_gdt_table()
{
	struct gdt g;
	_setup_gdt_table(gdt);
	g.address = (unsigned int)(void*)gdt;
	g.size = sizeof(gdt);
	return g;
}

