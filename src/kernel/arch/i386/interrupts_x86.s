.intel_syntax noprefix

.global interrupt
.align   4
interrupt:
	push    [esp+4]    # load the interrupt number into register eax
    int   	0x0                    # push the interrupt number
    ret

