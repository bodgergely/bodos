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

#define KERNEL_ENTRY_IN_PAGEDIR 768
#define NUM_KERNEL_PAGES_AT_BOOT 4
#define PAGE_ENTRY_MASK 0xFFFFF000
#define page_entry(entry) (entry & PAGE_ENTRY_MASK)


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



struct page_table_info
{
	page_table_t* table;
	uint16_t	  num_of_free_pages;
};

struct pd_info
{
	struct page_table_info entries[NUM_OF_PDE];
};


static void init_pagetable_info(struct page_table_info* info)
{
	// all pages are free in the beginning
	info->num_of_free_pages = NUM_OF_PTE;
}


struct page_directory_info
{
	page_directory_t* table;
	struct pd_info info;
};

// 1024 - 768 page tables (768th entry is allocated in boot.s - for the kernel)
page_table_t kernel_page_tables[NUM_OF_PTE-KERNEL_ENTRY_IN_PAGEDIR];
static struct page_directory_info pagedir_info;


static void init_higher_half_pagedirectory_info(struct page_directory_info* pdi)
{
	struct pd_info* info = &pdi->info;
	page_directory_t* pdt = pdi->table;
	uint32_t addr= VIRTUAL_MEM_OFFSET + PAGE_SIZE*1024;
	for(int i=0;i<KERNEL_ENTRY_IN_PAGEDIR;i++)
	{
		pdt->entries[i] = 0;
	}

	for(int i=0;i<NUM_KERNEL_PAGES_AT_BOOT;i++)
	{
		info->entries[KERNEL_ENTRY_IN_PAGEDIR+i].table = (&boot_page_table1) + PAGE_SIZE*i;
		info->entries[KERNEL_ENTRY_IN_PAGEDIR+i].num_of_free_pages = 0;
	}

	for(int i=KERNEL_ENTRY_IN_PAGEDIR+NUM_KERNEL_PAGES_AT_BOOT;i<NUM_OF_PDE;i++)
	{
		info->entries[i].num_of_free_pages = NUM_OF_PTE;
		pdt->entries[i] = ((uint32_t)(kernel_page_tables + i)) & 0x003;
		info->entries[i].table = kernel_page_tables + i;
		for(int j=0;j<NUM_OF_PTE;j++)
		{
			//klog(INFO, "Hihi %d\n", virtual_to_physical(info->entries[i].table->entries));
			info->entries[i].table->entries[j] = virtual_to_physical(addr) | 0x003;
			//klog(INFO, "Here\n");
			addr+=PAGE_SIZE;
		}
	}
}


void paging_init()
{

	pagedir_info.table = &boot_page_directory;
	init_higher_half_pagedirectory_info(&pagedir_info);


	page_directory_t* pagedir = pagedir_info.table;

	klog(INFO, "page el at 768 is: %d\n", page_entry(phys_to_virtual(pagedir->entries[KERNEL_ENTRY_IN_PAGEDIR])));
	klog(INFO, "page table is at: %d\n", page_entry((uint32_t) &boot_page_table1));


}


void* alloc_page(size_t count)
{
	int num_pages_collected = 0;
	struct pd_info* pdi = &pagedir_info.info;
	for(int i=KERNEL_ENTRY_IN_PAGEDIR+1;i<NUM_OF_PDE;i++)
	{
		if(pdi->entries[i].num_of_free_pages >= 0)
		{

		}
	}

}

void  free_page(void* page)
{

}

