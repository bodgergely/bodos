.intel_syntax noprefix

.global  load_gdt

# load_gdt - Loads the interrupt descriptor table (IDT).
# stack: [esp + 4] the address of the gdt register
load_gdt:
    mov     eax, [esp+4]    # load the address of the IDT into register eax
    lgdt    [eax]           # load the IDT
    ret                     # return to the calling function
