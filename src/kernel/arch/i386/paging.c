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
	uint8_t	 taken;
};





#define NUM_DYNAMIC_KERNEL_PAGE_TABLES (NUM_OF_PTE-KERNEL_ENTRY_IN_PAGEDIR-NUM_KERNEL_PAGE_TABLES_AT_BOOT)
static struct frame phys_frames[1024 * 1024];
// 1024 - 768 page tables (768th entry is allocated in boot.s - for the kernel)
page_table_t kernel_page_tables[NUM_DYNAMIC_KERNEL_PAGE_TABLES];
static struct page_directory_info pagedir_info;
static struct frame* free_frame_list;
static struct frame* free_kernel_frame_list;


static void init_higher_half_pagedirectory_info(struct page_directory_info* pdi)
{
	struct pd_info* directory = &pdi->info;
	page_directory_t* pdt = pdi->table;

	for(int i=0;i<KERNEL_ENTRY_IN_PAGEDIR;i++)
	{
		pdt->entries[i] = 0;
	}

	for(int i=1024*768;i<1024*768+1024*NUM_DYNAMIC_KERNEL_PAGE_TABLES;i++)
		phys_frames[i].taken = TRUE;
	for(int i=0;i<NUM_KERNEL_PAGE_TABLES_AT_BOOT;i++)
	{
		struct page_table_info* page_table = &directory->entries[KERNEL_ENTRY_IN_PAGEDIR+i];
		page_table->table = (&boot_page_table1) + PAGE_SIZE*i;
		page_table->num_of_free_pages = 0;
		// assign physical addresses for them
		uint32_t paddr = PHYSICAL_MEM_OFFSET + i * PAGE_SIZE*1024;
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


static struct frame* kernel_free_list()
{
	// the idea is to skip the frames page tables allocated at boot time since those frames in those tables are definitely taken
	struct frame* frame = free_frame_list;
	struct frame* bFree;
	for(int i=0;i<1024 * 768 - 1;i++)
	{
		//if(i > 400000)
		//	klog(INFO, "i %d frame: %d frame->next: %d\n", i, frame, frame->next);
		frame = frame->next;
	}
	bFree = frame;
	for(int i=0;i<1024*NUM_KERNEL_PAGE_TABLES_AT_BOOT;i++)
		frame = frame->next;
	bFree->next = frame->next;
	return bFree;
}

static void init_frames()
{
	for(uint32_t i=0;i<1024 * 1024;i++)
	{
		phys_frames[i].addr = i * 1024;
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
	free_kernel_frame_list = kernel_free_list();


	page_directory_t* pagedir = pagedir_info.table;

	klog(INFO, "page el at 768 is: %d\n", page_entry(phys_to_virtual(pagedir->entries[KERNEL_ENTRY_IN_PAGEDIR])));
	klog(INFO, "page table is at: %d\n", page_entry((uint32_t) &boot_page_table1));


}


void* alloc_pages(size_t count)
{
	int num_pages_collected = 0;
	int doable = 1;
	struct pd_info* pdi = &pagedir_info.info;

	while(num_pages_collected!=count && doable)
	{
		for(int i=KERNEL_ENTRY_IN_PAGEDIR+1;i<NUM_OF_PDE;i++)
		{
			struct page_table_info* page_table = &pdi->entries[i];
			if(page_table->num_of_free_pages >= num_pages_collected)
			{
				//page_table->
			}
		}
	}

}

void  free_pages(void* start, size_t count)
{

}

