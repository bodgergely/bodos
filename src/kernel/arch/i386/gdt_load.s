.intel_syntax noprefix

.section .data
gdtr:
.hword 0  # For limit storage
.word 0  # For base storage

msg:
.asciz "address: %d\0"

.section .text
.global  load_gdt

# load_gdt - Loads the global descriptor table (GDT).
# stack: [esp + 4] the address of the gdt register
load_gdt:
    mov     eax, [esp+4]    # load the address of the GDT into register eax
    lgdt    [eax]           # load the IDT
    ret                     # return to the calling function



.global  setGdt
setGdt:
   mov   ecx, [ESP + 8]
   push  ecx
   mov   ecx, [ESP + 8]
   push   ecx
   lea    ecx, msg
   push	  ecx
   push	  1
   push   1
   call   kprintf
   #MOV   EAX, [esp + 4]
   #MOV   [gdtr + 2], EAX
   #MOV   AX, [ESP + 8]
   #MOV   [gdtr], AX
   #mov   ecx, [ESP + 8]
   #push  ecx
   #mov   ecx, [ESP + 8]
   #push   ecx
   #lea    ecx, msg
   #push	  ecx
   #push	  1
   #push   1
   #call   kprintf
   #LGDT  [gdtr]
   RET




