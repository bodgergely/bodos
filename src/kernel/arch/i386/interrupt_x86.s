.intel_syntax noprefix

.section .data
debug_msg:
	.asciz "Just got back from int!"

.global interrupt
.align   4
interrupt:
	mov     eax, [esp+4]	# load the interrupt number into register eax
	push 	eax				# push the interrupt number
    int   	0x0
    #debug
	#lea    ecx, debug_msg
   	#push   ecx
   	#push   1
   	#push   1
   	#call   kprintf
   	#end of debug
   	pop eax
   	ret

