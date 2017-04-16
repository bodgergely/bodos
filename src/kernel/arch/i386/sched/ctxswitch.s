.intel_syntax noprefix


.section .data
debug_msg:
	.asciz "Mem location where Sp will be saved: %d and Old SP: %d\n"


.global ctxswitch
ctxswitch:
	push ebp		# push ebp onto stack
	mov	 ebp, esp	# record current sp in ebp
	pushf			# push flags onto the stack
	pusha			# push general regs on stack

	mov	eax, [ebp + 8]		# get the mem location in which to save the old process's sp
	mov [eax], esp			# save the old process's sp

	#debug
	push 	esp
	push    eax
	lea    ecx, debug_msg
   	push   ecx
   	push   0
   	push   1
   	push   1
   	call   kprintf_level
   	#end of debug

#looping:
#	jmp looping

	mov	eax, [ebp + 12]		# get location from which to restore new process's sp


	# we switch here to the new process's stack
	mov esp, [eax]			# load the new process's sp

	popa					# restore general registers
	mov ebp, [esp + 4]		# pick up ebp before restoring interrupts (ebp was pushed onto the stack in the beginning)
	popf					# restore interrupt mask
	add	esp, 4				# skip saved value of ebp
	ret						# return to new process
