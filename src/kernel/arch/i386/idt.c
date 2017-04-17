#include "idt.h"

#define NUM_OF_IDT_ENTRIES 3
#define IDT_ENTRY_SIZE 8

short girmask = 0xfffb;

struct idt_entry			// http://wiki.osdev.org/Interrupt_Descriptor_Table
{
	uint16_t low_addr;	// offset bits 0..15
	uint16_t selector;	// a code segment selector in GDT or LDT
	uint8_t  zero;		// unused, set to 0
	uint8_t type_attr;	// type and attributes
	uint16_t high_addr;  // offset bits 16..31
}__attribute__((packed));

struct idtr
{
	uint16_t limit;
	uint32_t base;
}__attribute__((packed));

extern "C" void load_idt(uint64_t*);

struct idt_entry idt[256];
struct idtr		 idtptr;

inline static void set_address(struct idt_entry* entry, uint32_t handler)
{
	entry->low_addr = (uint16_t)(handler & 0x0000ffff);
	entry->high_addr = (uint16_t)(handler >> 16);
}

inline static void set_selector(struct idt_entry* entry, uint16_t val)
{
	entry->selector = val;
}

inline static void set_attributes(struct idt_entry* entry, uint8_t val)
{
	entry->type_attr = val;
}

inline static void set_zero(struct idt_entry* entry)
{
	entry->zero = 0;
}

void create_idt_entry(size_t index, uint32_t handler, uint16_t selector, uint8_t type_attr)
{
	struct idt_entry entry;
	set_address(&entry, handler);
	set_selector(&entry, selector);	// https://littleosbook.github.io/#segmentation
	set_zero(&entry);
	set_attributes(&entry, type_attr);
	idt[index] = entry;
}

void idt_install()
{
	memset(idt, 0, sizeof(idt));
	idtptr.base = reinterpret_cast<uint32_t>(idt);
	idtptr.limit = sizeof(idt)-1;
	load_idt((uint64_t*)&idtptr);
}
