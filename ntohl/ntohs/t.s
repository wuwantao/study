	.file	"t.c"
	.section	.rodata
.LC0:
	.string	"malloc failed."
.LC1:
	.string	"ntohl time:%ld\n"
.LC2:
	.string	"NTOHL time:%ld\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$96, %rsp
	movl	$80000000, %edi
	call	malloc
	movq	%rax, -32(%rbp)
	movq	$0, -40(%rbp)
	cmpq	$0, -32(%rbp)
	jne	.L2
	movl	$.LC0, %edi
	call	puts
	movl	$-1, %eax
	jmp	.L10
.L2:
	movq	$0, -8(%rbp)
	jmp	.L4
.L5:
	call	random
	movq	%rax, %rcx
	movq	-32(%rbp), %rax
	movq	-8(%rbp), %rdx
	movq	%rcx, (%rax,%rdx,8)
	addq	$1, -8(%rbp)
.L4:
	movq	-8(%rbp), %rax
	cmpq	$9999999, %rax
	jbe	.L5
	leaq	-80(%rbp), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	gettimeofday
	movq	-80(%rbp), %rax
	imulq	$1000000, %rax, %rdx
	movq	-72(%rbp), %rax
	addq	%rdx, %rax
	movq	%rax, -48(%rbp)
	movq	$0, -16(%rbp)
	jmp	.L6
.L7:
	movq	-32(%rbp), %rax
	movq	-16(%rbp), %rdx
	movq	(%rax,%rdx,8), %rax
	movl	%eax, %edi
	call	ntohl
	movl	%eax, %ecx
	movq	-32(%rbp), %rax
	movq	-16(%rbp), %rdx
	movq	%rcx, (%rax,%rdx,8)
	addq	$1, -16(%rbp)
.L6:
	movq	-16(%rbp), %rax
	cmpq	$9999999, %rax
	jbe	.L7
	leaq	-96(%rbp), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	gettimeofday
	movq	-96(%rbp), %rax
	imulq	$1000000, %rax, %rdx
	movq	-88(%rbp), %rax
	addq	%rdx, %rax
	movq	%rax, -56(%rbp)
	movq	-48(%rbp), %rax
	movq	-56(%rbp), %rdx
	subq	%rax, %rdx
	movq	%rdx, %rax
	movq	%rax, %rsi
	movl	$.LC1, %edi
	movl	$0, %eax
	call	printf
	leaq	-80(%rbp), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	gettimeofday
	movq	-80(%rbp), %rax
	imulq	$1000000, %rax, %rdx
	movq	-72(%rbp), %rax
	addq	%rdx, %rax
	movq	%rax, -48(%rbp)
	movq	$0, -24(%rbp)
	jmp	.L8
.L9:
	movq	-32(%rbp), %rax
	movq	-24(%rbp), %rdx
	movq	(%rax,%rdx,8), %rax
#APP
# 45 "t.c" 1
	bswap %rax
# 0 "" 2
#NO_APP
	movq	-32(%rbp), %rdx
	movq	-24(%rbp), %rcx
	movq	%rax, (%rdx,%rcx,8)
	addq	$1, -24(%rbp)
.L8:
	movq	-24(%rbp), %rax
	cmpq	$9999999, %rax
	jbe	.L9
	leaq	-96(%rbp), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	gettimeofday
	movq	-96(%rbp), %rax
	imulq	$1000000, %rax, %rdx
	movq	-88(%rbp), %rax
	addq	%rdx, %rax
	movq	%rax, -56(%rbp)
	movq	-48(%rbp), %rax
	movq	-56(%rbp), %rdx
	subq	%rax, %rdx
	movq	%rdx, %rax
	movq	%rax, %rsi
	movl	$.LC2, %edi
	movl	$0, %eax
	call	printf
	movl	$0, %eax
.L10:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	main, .-main
	.ident	"GCC: (GNU) 4.8.5 20150623 (Red Hat 4.8.5-39)"
	.section	.note.GNU-stack,"",@progbits
