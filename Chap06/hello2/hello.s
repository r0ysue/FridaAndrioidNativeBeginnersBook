	.text
	.syntax unified
	.fpu	neon
	.file	"hello.c"
	.globl	main                            @ -- Begin function main
	.p2align	2
	.type	main,%function
	.code	32                              @ @main
main:
	push {lr}
	ldr r0,[r1,#8]
	bl printf
	mov r0,#0
	pop {lr}
	bx lr