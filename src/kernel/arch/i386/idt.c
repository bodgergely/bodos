#include "idt.h"
#include "interrupt_x86.h"

#define NUM_OF_IDT_ENTRIES 3
#define IDT_ENTRY_SIZE 8

extern interrupt_handler_signature interrupt_handler_0;

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

inline static void set_attributes(struct idt_entry* entry, uint16_t val)
{
	entry->type_attr = val;
}

inline static void set_zero(struct idt_entry* entry)
{
	entry->zero = 0;
}

static void create_idt_entry(size_t index, uint32_t handler)
{
	struct idt_entry entry;
	set_address(&entry, handler);
	set_selector(&entry, 0x0008);	// https://littleosbook.github.io/#segmentation
	set_zero(&entry);
	set_attributes(&entry, 0x8E);
	idt[index] = entry;
}

uint64_t* setup_idt_table()
{
	kprintf(INFO, FRAME_BUFFER, "interrupthandler_0: %d size of idt: %d\n", (uint32_t)interrupt_handler_0, sizeof(idt));
	memset(idt, 0, sizeof(idt));
	create_idt_entry(0x0, (uint32_t)interrupt_handler_0);
	idtptr.base = idt;
	idtptr.limit = sizeof(idt)-1;
	return (uint64_t*)&idtptr;
}
