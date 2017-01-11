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
#define NUM_KERNEL_PAGE_TABLES_AT_BOOT 8
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
	uint8_t		  taken[NUM_OF_PTE];
};

struct pd_info
{
	struct page_table_info entries[NUM_OF_PDE];
};


static void init_pagetable_info(struct page_table_info* info)
{
	// all pages are free in the beginning
	info->table = NULL;
	info->num_of_free_pages = NUM_OF_PTE;
	memset(info->taken, FALSE, sizeof(info->taken));
}


struct page_directory_info
{
	page_directory_t* table;
	struct pd_info info;
};




struct frame
{
	uint32_t addr;
	struct frame* next;
	uint32_t	 taken;
};





#define NUM_DYNAMIC_KERNEL_PAGE_TABLES (NUM_OF_PTE-KERNEL_ENTRY_IN_PAGEDIR-NUM_KERNEL_PAGE_TABLES_AT_BOOT)
// 1024 - 768 page tables (768th entry is allocated in boot.s - for the kernel)
page_table_t kernel_page_tables[NUM_DYNAMIC_KERNEL_PAGE_TABLES]__attribute__((aligned(4096)));
static struct page_directory_info pagedir_info;


inline static uint32_t virtual_addr(uint16_t pde_index, uint16_t pte_index)
{
	uint32_t high = PAGE_SIZE * NUM_OF_PTE * pde_index;
	uint32_t low = PAGE_SIZE * pte_index;
	return high + low;
}

static void init_higher_half_pagedirectory_info(struct page_directory_info* pdi)
{
	struct pd_info* directory = &pdi->info;
	page_directory_t* pdt = pdi->table;

	for(int i=0;i<KERNEL_ENTRY_IN_PAGEDIR;i++)
	{
		pdt->entries[i] = 0;
	}

	for(int i=0;i<NUM_KERNEL_PAGE_TABLES_AT_BOOT;i++)
	{
		struct page_table_info* page_table = &directory->entries[KERNEL_ENTRY_IN_PAGEDIR+i];
		page_table->table = (&boot_page_table1) + NUM_OF_PTE*i;
		page_table->num_of_free_pages = 0;
	}

	uint32_t vaddr = VIRTUAL_MEM_OFFSET + NUM_KERNEL_PAGE_TABLES_AT_BOOT * PAGE_SIZE*1024;
	uint32_t slot = 0;
	for(int i=KERNEL_ENTRY_IN_PAGEDIR+NUM_KERNEL_PAGE_TABLES_AT_BOOT;i<NUM_OF_PDE;i++)
	{
		struct page_table_info* page_table = &directory->entries[i];
		page_table->num_of_free_pages = NUM_OF_PTE;
		pdt->entries[i] = virtual_to_physical(((uint32_t)(kernel_page_tables + slot))) + 0x003;
		page_table->table = kernel_page_tables + slot;
		for(int j=0;j<NUM_OF_PTE;j++)
		{
			page_table->table->entries[j] = virtual_to_physical(vaddr) + 0x003;
			vaddr+=PAGE_SIZE;
		}
		slot++;
	}
	tlb_flush();

}


void paging_init()
{
	klog(INFO, "Initializing paging.\n");
	pagedir_info.table = &boot_page_directory;
	for(int i=0;i<NUM_DYNAMIC_KERNEL_PAGE_TABLES;i++)
		init_pagetable_info(kernel_page_tables + i);

	init_higher_half_pagedirectory_info(&pagedir_info);
	page_directory_t* pagedir = pagedir_info.table;

	//klog(INFO, "page el at 768 is: %d\n", page_entry(phys_to_virtual(pagedir->entries[KERNEL_ENTRY_IN_PAGEDIR])));
	//klog(INFO, "page table is at: %d\n", page_entry((uint32_t) &boot_page_table1));

	klog(INFO,"Paging has been initialized.\n");

}


void* alloc_pages(size_t count)
{
	uint16_t pde_start_index;
	uint16_t pte_start_index;
	int possible = TRUE;
	int num_pages_collected = 0;
	struct pd_info* pdi = &pagedir_info.info;

	while(num_pages_collected!=count && possible)
	{
		// determine where we can have consecutive count pages
		int page_table_crosses = 0;
		for(int pde=KERNEL_ENTRY_IN_PAGEDIR+NUM_KERNEL_PAGE_TABLES_AT_BOOT;pde<NUM_OF_PDE;pde++)
		{
			struct page_table_info* page_table = &pdi->entries[pde];
			if(page_table->num_of_free_pages)
			{
				int i = 0;
				while(i<NUM_OF_PTE)
				{
					pte_start_index = i;
					int keepGoing = TRUE;
					int c = 0;
					int sofarCount = num_pages_collected;
					for(;i<NUM_OF_PTE && sofarCount + c < count && keepGoing;i++)
					{
						if(!page_table->taken[i])
						{
							c++;
						}
						else
						{
							keepGoing = FALSE;
							if(page_table_crosses > 0)
							{
								num_pages_collected = 0;
								sofarCount = 0;
								c = 0;
								page_table_crosses = 0;
							}
						}
					}
					if(keepGoing)
					{
						num_pages_collected = sofarCount + c;
						if(num_pages_collected == count)
							break;
					}
				}
				if(num_pages_collected == count)
				{
					pde_start_index = pde - page_table_crosses;
					break;
				}
				else
				{
					page_table_crosses++;
				}

			}
			if(pde == NUM_OF_PDE)
				possible = FALSE;
		}
	}

	if(possible)
		return (void*) virtual_addr(pde_start_index, pte_start_index);
	else
		return NULL;

}

void  free_pages(void* start, size_t count)
{

}


int stress_test_page_alloc()
{
	char buff[256];
	int num_pages_to_allocate = 5;
	void* mem = alloc_pages(num_pages_to_allocate);
	klog(INFO, "Allocated %d page(s) at: %d\n", num_pages_to_allocate, mem);
	strcpy((char*)mem, "Bodos rules forever!");
	klog(INFO, "Mem we allocated at: %d contains: %s\n", mem, (char*)mem);
}

