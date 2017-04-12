.intel_syntax noprefix

.global ctxswitch

ctxswitch:
	mov eax, [esp + 8]    # code
    mov ebp, [esp + 4]    # stack
    mov esp, ebp
    call    eax
