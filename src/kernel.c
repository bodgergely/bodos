
#include "header.h"
#include "terminal.h"
#include "serialport.h"
#include "string.h"
 

 
#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
void kernel_main(void) {
	/* Initialize terminal interface */
	terminal_initialize();
 
	terminal_writestring("Starting kernel...");
	serial_write("Hello Marcika ugyes\n", strlen("Hello Marcika ugyes\n"));

}
