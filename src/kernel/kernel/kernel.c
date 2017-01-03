
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

volatile unsigned int g = 0xfffffff;

static void init()
{
	/* Initialize terminal interface */
	terminal_initialize();
	klog(INFO, "Terminal initialized.\n");
	arch_specific_init();
}

void kernel_main(void) {
	//while(1);
	init();

	klog(INFO, "kernel_main at: %d and init at: %d\n", kernel_main, init);
	int int_no = 28;
	klog(INFO, "Interrupt will fire now with int no: %d\n", int_no);
	interrupt(int_no);
	klog(INFO,"Returned from interrupt!\n");
	klog(INFO,"System will go into infinite loop now.\n");
	while(1);
}
