; FPU assembly functions located here ;



global  sqSQRT
global fpuSQRT

section .bss
    memory: resq 1

section .text

fpuSQRT:

	movq [memory], XMM0

	fsqrt

	fistp QWORD [memory]
	
	mov rax, [memory]

	ret


sqSQRT:
	sqrtsd XMM0, XMM0
	ret