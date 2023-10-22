	.text
	.syntax unified
	.fpu	neon
	.file	"hello.c"
	.globl	main                            @ -- Begin function main
	.p2align	2
	.type	main,%function
	.code	32                              @ @main
main:
	push	{r11, lr}
	mov	r11, sp
	sub	sp, sp, #8
	ldr	r0, .LCPI0_0
.LPC0_0:
	add	r0, pc, r0
	movw	r1, #0
	str	r1, [sp, #4]
	bl	printf
	movw	r1, #0
	str	r0, [sp]                        @ 4-byte Spill
	mov	r0, r1
	mov	sp, r11
	pop	{r11, pc}
