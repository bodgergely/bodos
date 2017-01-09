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
	uint32_t	  physical_frame_addresses[NUM_OF_PTE];
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
	memset(info->physical_frame_addresses, 0, sizeof(info->physical_frame_addresses));
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
static struct frame phys_frames[1024 * 1024];
// 1024 - 768 page tables (768th entry is allocated in boot.s - for the kernel)
page_table_t kernel_page_tables[NUM_DYNAMIC_KERNEL_PAGE_TABLES];
static struct page_directory_info pagedir_info;
static struct frame* free_frame_list;
static struct frame* free_kernel_frame_list;


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

	for(int i=0;i<1024*NUM_KERNEL_PAGE_TABLES_AT_BOOT;i++)
		phys_frames[i].taken = TRUE;

	for(int i=0;i<NUM_KERNEL_PAGE_TABLES_AT_BOOT;i++)
	{
		struct page_table_info* page_table = &directory->entries[KERNEL_ENTRY_IN_PAGEDIR+i];
		page_table->table = (&boot_page_table1) + PAGE_SIZE*i;
		page_table->num_of_free_pages = 0;
		// assign physical addresses for them
		uint32_t paddr = /*PHYSICAL_MEM_OFFSET + */i * PAGE_SIZE*1024;
		for(int i=0;i<NUM_OF_PTE;i++)
		{
			page_table->physical_frame_addresses[i] = paddr + i * PAGE_SIZE;
		}

	}

	uint32_t vaddr = VIRTUAL_MEM_OFFSET + NUM_KERNEL_PAGE_TABLES_AT_BOOT * PAGE_SIZE*1024;
	for(int i=KERNEL_ENTRY_IN_PAGEDIR+NUM_KERNEL_PAGE_TABLES_AT_BOOT;i<NUM_OF_PDE;i++)
	{
		uint32_t slot = 0;
		struct page_table_info* page_table = &directory->entries[i];
		page_table->num_of_free_pages = NUM_OF_PTE;
		pdt->entries[i] = ((uint32_t)(kernel_page_tables + slot)) & 0x003;
		directory->entries[i].table = kernel_page_tables + slot;
		for(int j=0;j<NUM_OF_PTE;j++)
		{
			page_table->table->entries[j] = virtual_to_physical(vaddr) | 0x003;
			vaddr+=PAGE_SIZE;
		}
		slot++;
	}
}


static struct frame* free_list()
{
	// the idea is to skip the frames page tables allocated at boot time since those frames in those tables are definitely taken
	struct frame* frame = free_frame_list;
	for(int i=0;i<1024 * NUM_KERNEL_PAGE_TABLES_AT_BOOT;i++)
	{
		//if(i > 400000)
		//	klog(INFO, "i %d frame: %d frame->next: %d\n", i, frame, frame->next);
		frame = frame->next;
	}

	return frame;
}

static void init_frames()
{
	for(uint32_t i=0;i<1024 * 1024;i++)
	{
		phys_frames[i].addr = i * PAGE_SIZE;
		phys_frames[i].taken = FALSE;
		if(i<1024 * 1024 - 1)
		{
			phys_frames[i].next = &phys_frames[i+1];
		}
		else
		{
			phys_frames[i].next = NULL;
		}
	}
	free_frame_list = phys_frames;
}

void paging_init()
{
	klog(INFO, "Paging init...\n");
	// frames are not taken in the beginning
	init_frames();
	pagedir_info.table = &boot_page_directory;
	for(int i=0;i<NUM_DYNAMIC_KERNEL_PAGE_TABLES;i++)
		init_pagetable_info(kernel_page_tables + i);

	init_higher_half_pagedirectory_info(&pagedir_info);
	free_kernel_frame_list = free_list();
	klog(INFO, "First free frame phys address: %d taken: %d next addr: %d next taken: %d\n", free_kernel_frame_list->addr, free_kernel_frame_list->taken, free_kernel_frame_list->next->addr, free_kernel_frame_list->next->taken);
	klog(INFO, "phys %d taken: %d\n", phys_frames[NUM_KERNEL_PAGE_TABLES_AT_BOOT*1024-1].addr, phys_frames[NUM_KERNEL_PAGE_TABLES_AT_BOOT*1024-1].taken);

	page_directory_t* pagedir = pagedir_info.table;

	klog(INFO, "page el at 768 is: %d\n", page_entry(phys_to_virtual(pagedir->entries[KERNEL_ENTRY_IN_PAGEDIR])));
	klog(INFO, "page table is at: %d\n", page_entry((uint32_t) &boot_page_table1));

	klog(INFO, "Paging has been initialized.\n");

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
	{
		// collect the frames from the free frame list
		struct frame* pF = free_kernel_frame_list;
		int processed = 0;
		int pde_index = pde_start_index;
		int pte_index = pte_start_index;
		while(processed!=count)
		{
			struct page_table_info* page_table = &pdi->entries[pde_index];
			page_table->taken[pte_index] = TRUE;
			page_table->num_of_free_pages--;
			page_table->physical_frame_addresses[pte_index] = pF->addr;
			page_table->table->entries[pte_index] = pF->addr;
			pF->taken = TRUE;
			pF = pF->next;
			free_kernel_frame_list = pF;

			if(pte_index == NUM_OF_PTE)
			{
				pte_index = 0;
				pde_index++;
			}
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

