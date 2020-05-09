
#include <kernel/header.h>
#include <kernel/tty.h>
#include <kernel/kprintf.h>
#include <kernel/init.h>
#include <string.h>
#include <kernel/debug.h>
#include <kernel/interrupt.h>
#include <kernel/mem/kmalloc.h>
#include <kernel/tests/TestCenter.h>
#include <kernel/proc/proc.h>
#include <kernel/sched/sched.h>
#include <kernel/utils/utils.h>

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif

volatile unsigned int g = 0xfffffff;

void runtests();

static void init()
{
	/* Initialize terminal interface */
	terminal_initialize();
	klog(INFO, "Terminal has been initialized.\n");
	arch_specific_init();
	memory::init();
    klog(INFO, "Heap has been initialized.\n");
	processes::init();
    klog(INFO, "Processes has been initialized.\n");
	scheduler::init();
    klog(INFO, "Scheduler has been initialized.\n");
}

extern "C" void kernel_main(void) {
	init();
	int int_no = 28;
	klog(INFO, "Interrupt will fire now with int no: %d\n", int_no);
	interrupt(int_no);
	klog(INFO,"Returned from interrupt!\n");

	klog(INFO, "Running tests..\n");
	runtests();
	klog(INFO, "Finished running tests.\n");

	klog(INFO,"System will go into infinite loop now.\n");
	HALT;
}


void runtests()
{
	TestCenter testcenter;
	testcenter.run();
}




