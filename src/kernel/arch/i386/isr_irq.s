.intel_syntax noprefix

/*
http://www.osdever.net/bkerndev/Docs/isrs.htm
*/

.section .data
debug_msg:
	.asciz "Just got back from interrupt_handler - old eip: %d old cs: %d old EFLAGS: %d\n"
debug_msg2:
	.asciz "Val: %d\n"

# load_idt - Loads the interrupt descriptor table (IDT).
# stack: [esp + 4] the address of the first entry in the IDT
#        [esp    ] the return address
.global  load_idt
load_idt:
    mov     eax, [esp+4]    # load the address of the IDT into register eax
    lidt    [eax]             # load the IDT
    ret                     # return to the calling function


.macro no_error_code_interrupt_handler num
.global interrupt_handler_\num
.align   4
interrupt_handler_\num:
	mov	   eax, [esp+20]		 # http://www.logix.cz/michal/doc/i386/chp09-06.htm#09-06-01-02   look at Figure 9-5. Stack Layout after Exception of Interrupt - need to look 20 bytes (5 wors above the esp)
	push   0                     # push 0 as error code
    push   eax                    # push the interrupt number
    jmp     isr_common_stub    # jump to the common handler
.endm

.macro error_code_interrupt_handler num
.global interrupt_handler_\num
.align   4
interrupt_handler_\num:
    push   \num                    # push the interrupt number
    jmp     isr_common_stub    # jump to the isr_common_stub
.endm


# This is our common ISR stub. It saves the processor state, sets
# up for kernel mode segments, calls the C-level fault handler,
# and finally restores the stack frame.
isr_common_stub:
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10   # Load the Kernel Data Segment descriptor!
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp   # Push us the stack
    push eax
    call    _interrupt_handler
    #mov eax, 	   _interrupt_handler
    #call eax       # A special call, preserves the 'eip' register
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8     # Cleans up the pushed error code and pushed ISR number
    iret           # pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!

no_error_code_interrupt_handler 0       # create handler for interrupt 0
no_error_code_interrupt_handler 1       # create handler for interrupt 1
error_code_interrupt_handler    7       # create handler for interrupt 7


# irq handler gateway
/*
http://www.osdever.net/bkerndev/Docs/irqs.htm
*/

.macro IRQ_ROUTINE num
.global irq_routine_\num
.align 4
# map them to IDT entry 32 to 47
irq_routine_\num:
    cli
    push 0    # Note that these don't push an error code on the stack:
                   # We need to push a dummy error code
    push (\num + 32)
    jmp irq_common_stub
.endm

IRQ_ROUTINE 0
IRQ_ROUTINE 1
IRQ_ROUTINE 2
IRQ_ROUTINE 3
IRQ_ROUTINE 4
IRQ_ROUTINE 5
IRQ_ROUTINE 6
IRQ_ROUTINE 7
IRQ_ROUTINE 8
IRQ_ROUTINE 9
IRQ_ROUTINE 10
IRQ_ROUTINE 11
IRQ_ROUTINE 12
IRQ_ROUTINE 13
IRQ_ROUTINE 14
IRQ_ROUTINE 15

.extern irq_handler

# This is a stub that we have created for IRQ based ISRs. This calls
# '_irq_handler' in our C code.
irq_common_stub:
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp
    push eax
    call irq_handler
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8
    iret


