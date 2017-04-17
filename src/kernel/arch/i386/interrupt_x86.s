.intel_syntax noprefix

#include "interrupt_x86.h"

.section .data
debug_msg:
	.asciz "Just got back from int!"

.global interrupt
.global	enable
.global	disable
.global	restore
.global getirmask


.align   4
interrupt:
	mov     eax, [esp+4]	# load the interrupt number into register eax
	push 	eax				# push the interrupt number
    int   	0x80
    #debug
	#lea    ecx, debug_msg
   	#push   ecx
   	#push   1
   	#push   1
   	#call   kprintf
   	#end of debug
   	pop eax
   	ret

	/*---------------------------------------------------------
 * enable all interrupts
 *---------------------------------------------------------
 */
enable:
	cli
	mov   ax, girmask
	out   0x21, al
	shr   ax, 8
	out   0xA1, al
	sti
	ret


/*---------------------------------------------------------
 * disable()    - disable interrupts, save old state in ps
 *---------------------------------------------------------
 */

disable:
	cli
	pushf
	in     al, 0xA1
	shl    ax, 8
	in	   al, 0x21
	not    ax             /* but APIC wants negated form */
	mov    dx, ax
	mov    al, 0xff
	outb   0xA1 , al
	mov    al, 0xff
	outb   0x21, al
	popf
	mov    ax, dx
	ret


/*---------------------------------------------------------
 * restore(ps)    - restore interrupts to value in ps
 * STATWORD ps    (ulong ps)
 *---------------------------------------------------------
 */
restore:
	cli
	pushf
	mov    eax, [esp + 8]
	not    ax
	and    ax, girmask
	out    0x21, al
	shr    ax, 8
	out    0xA1, al
	popf
	sti
	ret



	/*------------------------------------------------------------------------
 * getirmask(ps)  - return current interrupt mask in ps
 *------------------------------------------------------------------------
 */
getirmask:
	in	   al, 0xA1
	shl    ax, 8
	in     al, 0x21
	mov    edx, [esp + 4]
	mov    [edx], ax
	ret

