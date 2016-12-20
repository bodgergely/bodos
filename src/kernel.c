
#include "header.h"
#include "terminal.h"
 

 
#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
void kernel_main(void) {
	/* Initialize terminal interface */
	terminal_initialize();
 
	/* Newline support is left as an exercise. */
	terminal_writestring("Hello, kernel World!\n");
}
