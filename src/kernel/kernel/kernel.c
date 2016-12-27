
#include <kernel/header.h>
#include <kernel/tty.h>
#include <kernel/kprintf.h>
#include <kernel/init.h>
#include <kernel/interrupts.h>
#include <string.h>
#include <kernel/debug.h>

 
#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif

static void init()
{
	/* Initialize terminal interface */
	terminal_initialize();
	kprintf(INFO, FRAME_BUFFER, "Terminal initialized.\n");
    //kprintf(INFO, FRAME_BUFFER, (const char*)1056776, 56, 79);
	arch_specific_init();
}




void debug()
{
	volatile int k;
	k = 0;
	uint32_t mem = 0xDEADBE3F;
	kprintf(INFO, FRAME_BUFFER, "mem: %d\n", mem);
	uint32_t* m = &mem;
	memset(m, 0, 2);
	kprintf(INFO, FRAME_BUFFER, "mem address: %d\n", mem_printable(((char*)m+0)));
	kprintf(INFO, FRAME_BUFFER, "mem address: %d\n", mem_printable(((char*)m+1)));
	kprintf(INFO, FRAME_BUFFER, "mem address: %d\n", mem_printable(((char*)m+2)));
	kprintf(INFO, FRAME_BUFFER, "mem address: %d\n", mem_printable(((char*)m+3)));
}

void kernel_main(void) {
	init();

	kprintf(INFO,FRAME_BUFFER, "Doing interrupt now!, interrupt() address: %d\n", interrupt);
	interrupt(28);
	debug();

	while(1);
}
