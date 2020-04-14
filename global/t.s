	.file	"t.c"
	.globl	i
	.data
	.align 4
	.type	i, @object
	.size	i, 4
i:
	.long	1
	.comm	in4_addr,4,4
	.comm	in6_addr,16,16
	.comm	str1,128,32
	.globl	str2
	.align 32
	.type	str2, @object
	.size	str2, 128
str2:
	.string	"hello"
	.zero	122
	.section	.rodata
.LC0:
	.string	"192.168.1.1/24"
.LC1:
	.string	"inet_pton: 0x%x\n"
.LC2:
	.string	"ret:%d\n"
.LC3:
	.string	"inet_pton"
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
	subq	$16, %rsp
	movl	$0, -4(%rbp)
	movq	$.LC0, -16(%rbp)
	movl	in4_addr(%rip), %eax
	movl	%eax, %esi
	movl	$.LC1, %edi
	movl	$0, %eax
	call	printf
	movq	-16(%rbp), %rax
	movl	$in4_addr, %edx
	movq	%rax, %rsi
	movl	$2, %edi
	call	inet_pton
	movl	%eax, -4(%rbp)
	movl	-4(%rbp), %eax
	movl	%eax, %esi
	movl	$.LC2, %edi
	movl	$0, %eax
	call	printf
	movl	in4_addr(%rip), %eax
	movl	%eax, %esi
	movl	$.LC1, %edi
	movl	$0, %eax
	call	printf
	cmpl	$1, -4(%rbp)
	je	.L2
	movl	$.LC3, %edi
	call	perror
.L2:
	movl	$1, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	main, .-main
	.ident	"GCC: (GNU) 4.8.5 20150623 (Red Hat 4.8.5-36)"
	.section	.note.GNU-stack,"",@progbits
