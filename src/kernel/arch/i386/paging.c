#include "paging.h"

/*
 * .global boot_page_directory

boot_page_directory:
	.skip 4096
.global boot_page_table1
boot_page_table1:
	.skip 4096
 * */

// the below two variables are defined in boot.s and still represent physical addresses so we need to convert them to virtual using the offset below
extern uint32_t boot_page_directory;
extern uint32_t boot_page_table1;

#define PAGE_ENTRY_MASK 0xFFFFF000
#define page_entry(entry) (entry & PAGE_ENTRY_MASK)

page_directory_t* pagedir;

void paging_init()
{
	pagedir = &boot_page_directory;
	klog(INFO, "page el at 768 is: %d\n", page_entry(phys_to_virtual(pagedir->entries[768])));
	klog(INFO, "page table is at: %d\n", page_entry((uint32_t) &boot_page_table1));


}
