	.text
	.syntax unified	
	.fpu	neon	
	.file	"hello.c"
	.globl	main                            @ -- Begin function main
	.p2align	2
	.type	main,%function
	.code	16                              @ @main
	.thumb_func
main:
	push	{r7, lr}
	mov	r7, sp
	.pad	#16
	sub	sp, #16
	movs	r0, #0
	str	r0, [sp, #12]
	str	r0, [sp, #8]                    @ 4-byte Spill
	bl	getchar
	ldr	r1, .LCPI0_0
.LPC0_0:
	add	r1, pc
	str	r0, [sp, #4]                    @ 4-byte Spill
	mov	r0, r1
	bl	printf
	ldr	r1, [sp, #8]                    @ 4-byte Reload
	str	r0, [sp]                        @ 4-byte Spill
	mov	r0, r1
	add	sp, #16
	pop	{r7, pc}
	.p2align	2
.LCPI0_0:
	.long	.L.str-(.LPC0_0+4)
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cantunwind
	.fnend
                                        @ -- End function
	.type	.L.str,%object                  @ @.str
	.section	.rodata.str1.1,"aMS",%progbits,1
.L.str:
	.asciz	"Hello, World! \n"
	.size	.L.str, 16
	.section	".note.GNU-stack","",%progbits
