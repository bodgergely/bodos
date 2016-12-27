
#include <kernel/header.h>
#include <kernel/tty.h>
#include <kernel/kprintf.h>
#include <kernel/init.h>
#include <string.h>
#include <kernel/debug.h>
#include <kernel/interrupt.h>

 
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

void kernel_main(void) {
	init();

	int int_no = 28;
	kprintf(INFO,FRAME_BUFFER, "Interrupt will fire now with int no: %d\n", int_no);
	interrupt(int_no);
	kprintf(INFO, FRAME_BUFFER, "Returned from interrupt!\n");
	kprintf(INFO, FRAME_BUFFER, "System will go into infinite loop now.\n");
	while(1);
}
