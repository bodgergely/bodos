
#include <kernel/header.h>
#include <kernel/tty.h>
#include <kernel/kprintf.h>
#include <string.h>

 
#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
void kernel_main(void) {
	/* Initialize terminal interface */
	terminal_initialize();

	const char* name = "Geri";
	char buff[512];
	strcpy(buff, name);

	terminal_writestring("Starting kernel...\n");
	kprintf(INFO, FRAME_BUFFER, "kernel_main at: %p and terminal_writestring at: %p and my name is: %s and the first char is: %c and the name again:%s \n", kernel_main, terminal_writestring, name, name[0],buff);
}
