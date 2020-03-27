	.file	"t.c"
	.section	.rodata
.LC0:
	.string	"i:%d\n"
	.text
	.globl	test_loop
	.type	test_loop, @function
test_loop:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	$0, -8(%rbp)
#APP
# 11 "t.c" 1
	push %rax
# 0 "" 2
# 12 "t.c" 1
	push %rcx
# 0 "" 2
# 13 "t.c" 1
	xor %rax, %rax
# 0 "" 2
# 14 "t.c" 1
	xor %rcx, %rcx
# 0 "" 2
# 15 "t.c" 1
	mov $0x5, %rcx
# 0 "" 2
# 16 "t.c" 1
	s:
# 0 "" 2
# 17 "t.c" 1
	add $0x2, %rax
# 0 "" 2
# 18 "t.c" 1
	loop s
# 0 "" 2
# 19 "t.c" 1
	mov %rax, %rax
# 0 "" 2
#NO_APP
	movq	%rax, -8(%rbp)
#APP
# 20 "t.c" 1
	pop %rcx
# 0 "" 2
# 21 "t.c" 1
	pop %rax
# 0 "" 2
#NO_APP
	movq	-8(%rbp), %rax
	movq	%rax, %rsi
	movl	$.LC0, %edi
	movl	$0, %eax
	call	printf
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	test_loop, .-test_loop
	.section	.rodata
.LC1:
	.string	"01234567890123456789012\n"
	.text
	.globl	test_one_byte_cpy
	.type	test_one_byte_cpy, @function
test_one_byte_cpy:
.LFB3:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$24, %rsp
	movq	$.LC1, -8(%rbp)
	leaq	-144(%rbp), %rsi
	movl	$0, %eax
	movl	$16, %edx
	movq	%rsi, %rdi
	movq	%rdx, %rcx
	rep stosq
#APP
# 31 "t.c" 1
	push %rsi
# 0 "" 2
# 32 "t.c" 1
	push %rdi
# 0 "" 2
# 33 "t.c" 1
	push %rcx
# 0 "" 2
#NO_APP
	movq	-8(%rbp), %rax
#APP
# 34 "t.c" 1
	movq %rax, %rsi
# 0 "" 2
#NO_APP
	leaq	-144(%rbp), %rax
#APP
# 35 "t.c" 1
	movq %rax, %rdi
# 0 "" 2
# 36 "t.c" 1
	mov $14, %rcx
# 0 "" 2
# 37 "t.c" 1
	cld
# 0 "" 2
# 38 "t.c" 1
	rep movsd
# 0 "" 2
# 39 "t.c" 1
	pop %rcx
# 0 "" 2
# 40 "t.c" 1
	pop %rdi
# 0 "" 2
# 41 "t.c" 1
	pop %rsi
# 0 "" 2
#NO_APP
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	test_one_byte_cpy, .-test_one_byte_cpy
	.globl	test_four_byte_cpy
	.type	test_four_byte_cpy, @function
test_four_byte_cpy:
.LFB4:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$24, %rsp
	movq	$.LC1, -8(%rbp)
	leaq	-144(%rbp), %rsi
	movl	$0, %eax
	movl	$16, %edx
	movq	%rsi, %rdi
	movq	%rdx, %rcx
	rep stosq
#APP
# 50 "t.c" 1
	push %rax
# 0 "" 2
# 51 "t.c" 1
	push %rsi
# 0 "" 2
# 52 "t.c" 1
	push %rdi
# 0 "" 2
#NO_APP
	movq	-8(%rbp), %rax
#APP
# 53 "t.c" 1
	movq %rax, %rsi
# 0 "" 2
#NO_APP
	leaq	-144(%rbp), %rax
#APP
# 54 "t.c" 1
	movq %rax, %rdi
# 0 "" 2
# 56 "t.c" 1
	movl (%rsi), %eax
# 0 "" 2
# 57 "t.c" 1
	movl %eax, (%rdi)
# 0 "" 2
# 59 "t.c" 1
	movl 4(%rsi), %eax
# 0 "" 2
# 60 "t.c" 1
	movl %eax, 4(%rdi)
# 0 "" 2
# 62 "t.c" 1
	movl 8(%rsi), %eax
# 0 "" 2
# 63 "t.c" 1
	movl %eax, 8(%rdi)
# 0 "" 2
# 65 "t.c" 1
	movl 12(%rsi), %eax
# 0 "" 2
# 66 "t.c" 1
	movl %eax, 12(%rdi)
# 0 "" 2
# 68 "t.c" 1
	movl 16(%rsi), %eax
# 0 "" 2
# 69 "t.c" 1
	movl %eax, 16(%rdi)
# 0 "" 2
# 71 "t.c" 1
	movl 20(%rsi), %eax
# 0 "" 2
# 72 "t.c" 1
	movl %eax, 20(%rdi)
# 0 "" 2
# 74 "t.c" 1
	pop %rdi
# 0 "" 2
# 75 "t.c" 1
	pop %rsi
# 0 "" 2
# 76 "t.c" 1
	pop %rax
# 0 "" 2
#NO_APP
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE4:
	.size	test_four_byte_cpy, .-test_four_byte_cpy
	.globl	test_eight_byte_cpy
	.type	test_eight_byte_cpy, @function
test_eight_byte_cpy:
.LFB5:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$24, %rsp
	movq	$.LC1, -8(%rbp)
	leaq	-144(%rbp), %rsi
	movl	$0, %eax
	movl	$16, %edx
	movq	%rsi, %rdi
	movq	%rdx, %rcx
	rep stosq
	movq	$0, -16(%rbp)
#APP
# 88 "t.c" 1
	push %rax
# 0 "" 2
# 89 "t.c" 1
	push %rsi
# 0 "" 2
# 90 "t.c" 1
	push %rdi
# 0 "" 2
#NO_APP
	movq	-8(%rbp), %rax
#APP
# 91 "t.c" 1
	movq %rax, %rsi
# 0 "" 2
#NO_APP
	leaq	-144(%rbp), %rax
#APP
# 92 "t.c" 1
	movq %rax, %rdi
# 0 "" 2
# 94 "t.c" 1
	movq (%rsi), %rax
# 0 "" 2
# 95 "t.c" 1
	movq %rax, (%rdi)
# 0 "" 2
# 97 "t.c" 1
	movq 8(%rsi), %rax
# 0 "" 2
# 98 "t.c" 1
	movq %rax, 8(%rdi)
# 0 "" 2
# 100 "t.c" 1
	movq 16(%rsi), %rax
# 0 "" 2
# 101 "t.c" 1
	movq %rax, 16(%rdi)
# 0 "" 2
# 103 "t.c" 1
	pop %rdi
# 0 "" 2
# 104 "t.c" 1
	pop %rsi
# 0 "" 2
# 105 "t.c" 1
	pop %rax
# 0 "" 2
#NO_APP
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	test_eight_byte_cpy, .-test_eight_byte_cpy
	.globl	uint16_str
	.bss
	.align 8
	.type	uint16_str, @object
	.size	uint16_str, 8
uint16_str:
	.zero	8
	.section	.rodata
.LC2:
	.string	"uint16_str malloc failed."
.LC3:
	.string	"%d"
	.text
	.globl	init_str
	.type	init_str, @function
init_str:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$24, %rsp
	.cfi_offset 3, -24
	movl	$0, -20(%rbp)
	movl	$524288, %edi
	call	malloc
	movq	%rax, uint16_str(%rip)
	movq	uint16_str(%rip), %rax
	testq	%rax, %rax
	jne	.L6
	movl	$.LC2, %edi
	call	puts
.L6:
	movq	uint16_str(%rip), %rax
	movl	$524288, %edx
	movl	$0, %esi
	movq	%rax, %rdi
	call	memset
	movl	$0, -20(%rbp)
	jmp	.L7
.L8:
	movq	uint16_str(%rip), %rdx
	movl	-20(%rbp), %eax
	sall	$3, %eax
	movl	%eax, %eax
	leaq	(%rdx,%rax), %rcx
	movl	-20(%rbp), %eax
	movl	%eax, %edx
	movl	$.LC3, %esi
	movq	%rcx, %rdi
	movl	$0, %eax
	call	sprintf
	movq	uint16_str(%rip), %rax
	movl	-20(%rbp), %edx
	sall	$3, %edx
	movl	%edx, %edx
	addq	$7, %rdx
	leaq	(%rax,%rdx), %rbx
	movq	uint16_str(%rip), %rdx
	movl	-20(%rbp), %eax
	sall	$3, %eax
	movl	%eax, %eax
	addq	%rdx, %rax
	movq	%rax, %rdi
	call	strlen
	movb	%al, (%rbx)
	addl	$1, -20(%rbp)
.L7:
	cmpl	$65535, -20(%rbp)
	jbe	.L8
	addq	$24, %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	init_str, .-init_str
	.globl	test_c_add_port_num
	.type	test_c_add_port_num, @function
test_c_add_port_num:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$160, %rsp
	movl	%edi, %eax
	movw	%ax, -148(%rbp)
	leaq	-144(%rbp), %rsi
	movl	$0, %eax
	movl	$16, %edx
	movq	%rsi, %rdi
	movq	%rdx, %rcx
	rep stosq
	leaq	-144(%rbp), %rax
	movq	%rax, -8(%rbp)
	movq	uint16_str(%rip), %rax
	movzwl	-148(%rbp), %edx
	sall	$3, %edx
	movslq	%edx, %rdx
	addq	$7, %rdx
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbq	%al, %rdx
	movq	uint16_str(%rip), %rcx
	movzwl	-148(%rbp), %eax
	sall	$3, %eax
	cltq
	addq	%rax, %rcx
	movq	-8(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	memcpy
	movq	uint16_str(%rip), %rax
	movzwl	-148(%rbp), %edx
	sall	$3, %edx
	movslq	%edx, %rdx
	addq	$7, %rdx
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbq	%al, %rax
	addq	%rax, -8(%rbp)
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	test_c_add_port_num, .-test_c_add_port_num
	.globl	test_hb_add_port_num
	.type	test_hb_add_port_num, @function
test_hb_add_port_num:
.LFB8:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$28, %rsp
	movl	%edi, %eax
	movw	%ax, -148(%rbp)
	leaq	-144(%rbp), %rsi
	movl	$0, %eax
	movl	$16, %edx
	movq	%rsi, %rdi
	movq	%rdx, %rcx
	rep stosq
	leaq	-144(%rbp), %rax
	movq	%rax, -8(%rbp)
#APP
# 147 "t.c" 1
	push %rax
# 0 "" 2
# 148 "t.c" 1
	push %rsi
# 0 "" 2
# 149 "t.c" 1
	push %rdi
# 0 "" 2
#NO_APP
	movq	uint16_str(%rip), %rdx
	movzwl	-148(%rbp), %eax
	sall	$3, %eax
	cltq
	addq	%rdx, %rax
#APP
# 150 "t.c" 1
	movq %rax, %rsi
# 0 "" 2
#NO_APP
	movq	-8(%rbp), %rax
#APP
# 151 "t.c" 1
	movq %rax, %rdi
# 0 "" 2
# 153 "t.c" 1
	movq (%rsi), %rax
# 0 "" 2
# 154 "t.c" 1
	movq %rax, (%rdi)
# 0 "" 2
# 156 "t.c" 1
	pop %rdi
# 0 "" 2
# 157 "t.c" 1
	pop %rsi
# 0 "" 2
# 158 "t.c" 1
	pop %rax
# 0 "" 2
#NO_APP
	movq	uint16_str(%rip), %rax
	movzwl	-148(%rbp), %edx
	sall	$3, %edx
	movslq	%edx, %rdx
	addq	$7, %rdx
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbq	%al, %rax
	addq	%rax, -8(%rbp)
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	test_hb_add_port_num, .-test_hb_add_port_num
	.section	.rodata
	.align 8
.LC5:
	.string	"test_hb_add_port_num: %f seconds\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB9:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$208, %rsp
	movq	$0, -8(%rbp)
	movw	$0, -10(%rbp)
	leaq	-176(%rbp), %rsi
	movl	$0, %eax
	movl	$16, %edx
	movq	%rsi, %rdi
	movq	%rdx, %rcx
	rep stosq
	leaq	-176(%rbp), %rax
	movq	%rax, -184(%rbp)
	movl	$0, %eax
	call	init_str
	movw	$0, -10(%rbp)
	leaq	-176(%rbp), %rax
	movq	%rax, -184(%rbp)
	call	clock
	movq	%rax, -24(%rbp)
	movq	$0, -8(%rbp)
	jmp	.L12
.L13:
#APP
# 193 "t.c" 1
	push %rax
# 0 "" 2
# 194 "t.c" 1
	push %rsi
# 0 "" 2
# 195 "t.c" 1
	push %rdi
# 0 "" 2
#NO_APP
	movq	uint16_str(%rip), %rdx
	movzwl	-10(%rbp), %eax
	sall	$3, %eax
	cltq
	addq	%rdx, %rax
#APP
# 196 "t.c" 1
	movq %rax, %rsi
# 0 "" 2
#NO_APP
	movq	-184(%rbp), %rax
#APP
# 198 "t.c" 1
	movq %rax, %rdi
# 0 "" 2
# 199 "t.c" 1
	add $7, %rdi
# 0 "" 2
#NO_APP
	leaq	-184(%rbp), %rax
#APP
# 201 "t.c" 1
	movq %rdi, (%rax)
# 0 "" 2
#NO_APP
	leaq	-176(%rbp), %rax
	movq	%rax, %rdx
#APP
# 205 "t.c" 1
	movq (%rsi), %rax
# 0 "" 2
# 206 "t.c" 1
	movq %rax, (%rdi)
# 0 "" 2
# 208 "t.c" 1
	pop %rdi
# 0 "" 2
# 209 "t.c" 1
	pop %rsi
# 0 "" 2
# 210 "t.c" 1
	pop %rax
# 0 "" 2
#NO_APP
	movzwl	-10(%rbp), %eax
	addl	$1, %eax
	movw	%ax, -10(%rbp)
	addq	$1, -8(%rbp)
.L12:
	movl	$2499999999, %eax
	cmpq	%rax, -8(%rbp)
	jbe	.L13
	call	clock
	movq	%rax, -32(%rbp)
	movq	-24(%rbp), %rax
	movq	-32(%rbp), %rdx
	subq	%rax, %rdx
	movq	%rdx, %rax
	cvtsi2sdq	%rax, %xmm0
	movsd	.LC4(%rip), %xmm1
	divsd	%xmm1, %xmm0
	movsd	%xmm0, -40(%rbp)
	movq	-40(%rbp), %rax
	movq	%rax, -200(%rbp)
	movsd	-200(%rbp), %xmm0
	movl	$.LC5, %edi
	movl	$1, %eax
	call	printf
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	main, .-main
	.section	.rodata
	.align 8
.LC4:
	.long	0
	.long	1093567616
	.ident	"GCC: (GNU) 4.8.5 20150623 (Red Hat 4.8.5-36)"
	.section	.note.GNU-stack,"",@progbits
