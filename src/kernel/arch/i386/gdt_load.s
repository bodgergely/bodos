.intel_syntax noprefix

.section .data
gdtr:
.hword 0  # For limit storage
.word 0  # For base storage

msg:
.asciz "address: %d\0"


/*
http://www.osdever.net/bkerndev/Docs/gdt.htm
*/

.section .text
.global  load_gdt
# load_gdt - Loads the global descriptor table (GDT).
# stack: [esp + 4] the address of the gdt register
load_gdt:
    mov     eax, [esp+4]    # load the address of the GDT into register eax
    lgdt    [eax]           # load the IDT
    mov ax, 0x10      # 0x10 is the offset in the GDT to our data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:flush   # 0x08 is the offset to our code segment: Far jump!
flush:
    ret               # Returns back to the C code!





