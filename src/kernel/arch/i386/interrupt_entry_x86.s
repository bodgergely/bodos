.intel_syntax noprefix



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
    push   \num                    # push the interrupt number
    push   0                     # push 0 as error code
    jmp     common_interrupt_handler    # jump to the common handler
.endm

.macro error_code_interrupt_handler num
.global interrupt_handler_\num
.align   4
interrupt_handler_\num:
    push   \num                    # push the interrupt number
    jmp     common_interrupt_handler    # jump to the common handler
.endm

common_interrupt_handler:               # the common parts of the generic interrupt handler
    # save the registers
    pushad		# https://pdos.csail.mit.edu/6.828/2011/readings/i386/PUSHA.htm
    cld			# C code following the sysV ABI requires DF to be clear on function entry
    # call the C function
    call    interrupt_handler

    # restore the registers
    popad

    # restore the esp
    add     esp, 8

    # return to the code that got interrupted
    iret

no_error_code_interrupt_handler 0       # create handler for interrupt 0
no_error_code_interrupt_handler 1       # create handler for interrupt 1
error_code_interrupt_handler    7       # create handler for interrupt 7

