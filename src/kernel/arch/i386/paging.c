#include "paging.h"

/*
 * .global boot_page_directory

boot_page_directory:
	.skip 4096
.global boot_page_table1
boot_page_table1:
	.skip 4096
 * */

void tlb_flush();

// the below two variables are defined in boot.s and still represent physical addresses so we need to convert them to virtual using the offset below
extern uint32_t boot_page_directory;
extern uint32_t boot_page_table1;

#define KERNEL_ENTRY_IN_PAGEDIR 768
#define NUM_KERNEL_PAGE_TABLES_AT_BOOT 8
#define PAGE_ENTRY_MASK 0xFFFFF000
#define page_entry(entry) (entry & PAGE_ENTRY_MASK)



inline uint32_t phys_to_virtual(uint32_t phys)
{
	return phys + (uint32_t)VIRTUAL_MEM_OFFSET;
}

inline uint32_t virtual_to_physical(uint32_t virt)
{
	return virt - (uint32_t)VIRTUAL_MEM_OFFSET;
}

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
		page_table->table = reinterpret_cast<page_table_t*>((&boot_page_table1) + NUM_OF_PTE*i);
		page_table->num_of_free_pages = 0;
	}

	uint32_t vaddr = VIRTUAL_MEM_OFFSET + NUM_KERNEL_PAGE_TABLES_AT_BOOT * PAGE_SIZE*1024;
	uint32_t slot = 0;
	for(int i=KERNEL_ENTRY_IN_PAGEDIR+NUM_KERNEL_PAGE_TABLES_AT_BOOT;i<NUM_OF_PDE;i++)
	{
		struct page_table_info* page_table = &directory->entries[i];
		page_table->num_of_free_pages = NUM_OF_PTE;
		memset(page_table->taken, FALSE, sizeof(page_table->taken));
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
	pagedir_info.table = reinterpret_cast<page_directory_t*>(&boot_page_directory);
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
			//klog(INFO, "pde: %d, page_table->num_of_free_pages: %d\n", pde, page_table->num_of_free_pages);
			if(page_table->num_of_free_pages)
			{
				int i = 0;
				int keepGoing = TRUE;
				while(i<NUM_OF_PTE)
				{
					// only modify the pte offset if we are not in crossing mode
					if(page_table_crosses == 0)
						pte_start_index = i;

					keepGoing = TRUE;
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
				else if(keepGoing)
				{
					page_table_crosses++;
				}

			}
			if(pde == NUM_OF_PDE)
				possible = FALSE;
		}
	}

	// mark the pages as taken if the allocation is possible
	//klog(INFO, "pde_start_index: %d pte_start_index: %d\n", pde_start_index, pte_start_index);
	if(possible)
	{
		int i = 0;
		uint16_t pde = pde_start_index;
		uint16_t pte = pte_start_index;
		while(i<count)
		{
			struct page_table_info* page_table = &pdi->entries[pde];
			//klog(INFO, "pte: %d and i: %d", pte, i);
			if(page_table->taken[pte])
			{
				klog(INFO, "Should not happen - inf loop now...\n");
				while(1);
			}
			page_table->taken[pte++] = TRUE;
			page_table->num_of_free_pages--;
			if(pte == NUM_OF_PTE)
			{
				pte = 0;
				pde++;
			}
			i++;
		}
	}

	if(possible)
		return (void*) virtual_addr(pde_start_index, pte_start_index);
	else
		return NULL;

}

static uint16_t calc_pd_offset(void* start)
{
	return (uint16_t)((uint32_t) start / (NUM_OF_PTE * PAGE_SIZE));
}

static uint16_t calc_pt_offset(void* start)
{
	uint32_t rem = (uint32_t)((uint32_t) start % (NUM_OF_PTE * PAGE_SIZE));
	return (uint16_t)rem / PAGE_SIZE;
}

void  free_pages(void* start, size_t count)
{
	uint16_t pd_offset = calc_pd_offset(start);
	uint16_t pt_offset = calc_pt_offset(start);
	struct pd_info* pdi = &pagedir_info.info;
	for(int i=0;i<count;i++)
	{
		struct page_table_info* page_table = &pdi->entries[pd_offset];
		page_table->taken[pt_offset++] = FALSE;
		page_table->num_of_free_pages++;
		if(pt_offset == NUM_OF_PTE)
		{
			pt_offset = 0;
			pd_offset++;
		}
	}
}

typedef struct alloc_info
{
	void* 	 location;
	uint32_t num_pages;
}alloc_info;

#define NUM_OF_MEMORIES 10
void do_test(int scaler_max, int scaler_step, int iter_count)
{
	alloc_info mem_arr[NUM_OF_MEMORIES];
	klog(INFO, "Performing %d allocs and frees.\n", scaler_max/scaler_step*iter_count*NUM_OF_MEMORIES);
	for(int scaler=1;scaler<scaler_max;scaler+=scaler_step)
	{
		for(int iter=0;iter<iter_count;iter++)
		{
			for(int j=0;j<NUM_OF_MEMORIES;j++)
			{
				uint32_t pc = 1 + (iter%5) * (j % 3) * scaler;
				mem_arr[j].location = alloc_pages(pc);
				mem_arr[j].num_pages = pc;
				//klog(INFO, "Allocated %d pages.\n", pc);
				strcpy((char*)mem_arr[j].location, "Something comes here.");
			}
			for(int j=0;j<NUM_OF_MEMORIES;j++)
			{
				free_pages(mem_arr[j].location, mem_arr[j].num_pages);
				//klog(INFO, "Freed %d pages.\n", mem_arr[j].num_pages);
			}
		}
	}
	klog(INFO, "Did %d allocations and frees!\n", scaler_max/scaler_step*iter_count*NUM_OF_MEMORIES);
}

void interactive_test()
{
	char buff[256];
		strcpy(buff, "Bodos has frame allocation now!");
		int num_pages_to_allocate = 5;
		char* mem = (char*)alloc_pages(num_pages_to_allocate);
		klog(INFO, "Allocated %d page(s) at: %d\n", num_pages_to_allocate, mem);
		strcpy(mem, buff);
		klog(INFO, "Mem we allocated at: %d contains: %s\n", mem, (char*)mem);
		char* mem_2 = (char*)alloc_pages(2);
		klog(INFO, "Allocated 2 pages at: %d\n", mem_2);
		strcpy(mem_2, mem);
		klog(INFO, "mem_2 at: %d contains : %s\n", mem_2, (char*)mem_2);
		klog(INFO, "Diff mem2 and mem: %d which is %d pages\n", mem_2 - (char*)mem, (mem_2 - (char*)mem)/PAGE_SIZE);
		klog(INFO, "Freeing %d pages at: %d\n", num_pages_to_allocate, mem);
		free_pages(mem, num_pages_to_allocate);

		int big_alloc_count = 1017;
		mem = (char*)alloc_pages(big_alloc_count);
		klog(INFO, "Allocated %d page(s) at: %d\n", big_alloc_count, mem);
		strcpy((char*)mem, buff);
		klog(INFO, "Mem we allocated at: %d contains: %s\n", mem, (char*)mem);
		klog(INFO, "Diff mem and mem2: %d whic is %d pages\n", (char*)mem - (char*)mem_2, ((char*)mem - (char*)mem_2) / PAGE_SIZE );


		void* mem_3 = alloc_pages(num_pages_to_allocate+13);
		strcpy((char*)mem_3, buff);
		klog(INFO, "Allocated %d at: %d\n", num_pages_to_allocate+13, mem_3);
		klog(INFO, "Mem we allocated at: %d contains: %s\n", mem_3, mem_3);
		klog(INFO, "Diff mem_3 and mem is %d pages\n", ((char*)mem_3 - (char*)mem) / PAGE_SIZE);


		klog(INFO, "freeing %d at: %d\n", big_alloc_count, mem);
		free_pages(mem, big_alloc_count);

		mem = (char*)alloc_pages(7);
		klog(INFO, "Allocated 7 page(s) at: %d\n", mem);
		klog(INFO, "Diff mem and mem_2: %d\n", (mem-mem_2)/PAGE_SIZE );

}

int stress_test_page_alloc()
{

	do_test(500, 10, 300);

}

