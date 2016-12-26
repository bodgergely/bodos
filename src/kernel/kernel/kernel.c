
#include <kernel/header.h>
#include <kernel/tty.h>
#include <kernel/kprintf.h>
#include <kernel/init.h>
#include <string.h>

 
#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif

static void init()
{
	/* Initialize terminal interface */
	terminal_initialize();
	kprintf(INFO, FRAME_BUFFER, "Terminal initialized.\n");
	arch_specific_init();
}

void kernel_main(void) {
	init();
}
