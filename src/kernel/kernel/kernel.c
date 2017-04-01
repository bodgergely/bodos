
#include <kernel/header.h>
#include <kernel/tty.h>
#include <kernel/kprintf.h>
#include <kernel/init.h>
#include <string.h>
#include <kernel/debug.h>
#include <kernel/interrupt.h>
#include <kernel/mem/kmalloc.h>

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif

volatile unsigned int g = 0xfffffff;

static void init()
{
	/* Initialize terminal interface */
	terminal_initialize();
	klog(INFO, "Terminal has been initialized.\n");
	arch_specific_init();
}

extern "C" void kernel_main(void) {
	init();
	int int_no = 28;
	klog(INFO, "Interrupt will fire now with int no: %d\n", int_no);
	interrupt(int_no);
	klog(INFO,"Returned from interrupt!\n");
	void* mem = kmalloc(60);
	klog(INFO,"System will go into infinite loop now.\n");
	while(1);
}
