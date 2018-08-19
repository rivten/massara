bits 32
section .text
	align 4
	dd 0x1BADB002
	dd 0x00
	dd - (0x1BADB002 + 0x00)

global start
extern kernel_main
extern KeyboardHandlerMain

ReadPort:
	mov edx, [esp + 4]
	in al, dx
	ret

WritePort:
	mov edx, [esp + 4]
	mov al, [esp + 4 + 4]
	out dx, al
	ret

LoadIDT:
	mov edx, [esp + 4]
	lidt [edx]
	sti
	ret

KeyboardHandler:
	call KeyboardHandlerMain
	iretd

start:
	cli
	mov esp, stack_space
	call kernel_main
	hlt

section .bss
resb 8192
stack_space:
