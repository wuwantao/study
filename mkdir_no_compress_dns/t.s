	.file	"t.c"
	.text
	.type	ldns_read_uint16, @function
ldns_read_uint16:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movzbl	(%rax), %eax
	movzbl	%al, %eax
	sall	$8, %eax
	movl	%eax, %edx
	movq	-8(%rbp), %rax
	addq	$1, %rax
	movzbl	(%rax), %eax
	movzbl	%al, %eax
	orl	%edx, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	ldns_read_uint16, .-ldns_read_uint16
	.comm	packet,2048,32
	.globl	pkt_len
	.bss
	.align 4
	.type	pkt_len, @object
	.size	pkt_len, 4
pkt_len:
	.zero	4
	.text
	.type	__rte_raw_cksum, @function
__rte_raw_cksum:
.LFB53:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movl	%edx, -36(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, -16(%rbp)
	movq	-16(%rbp), %rax
	movq	%rax, -8(%rbp)
	jmp	.L4
.L5:
	movq	-8(%rbp), %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %eax
	addl	%eax, -36(%rbp)
	movq	-8(%rbp), %rax
	addq	$2, %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %eax
	addl	%eax, -36(%rbp)
	movq	-8(%rbp), %rax
	addq	$4, %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %eax
	addl	%eax, -36(%rbp)
	movq	-8(%rbp), %rax
	addq	$6, %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %eax
	addl	%eax, -36(%rbp)
	subq	$8, -32(%rbp)
	addq	$8, -8(%rbp)
.L4:
	cmpq	$7, -32(%rbp)
	ja	.L5
	jmp	.L6
.L7:
	movq	-8(%rbp), %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %eax
	addl	%eax, -36(%rbp)
	subq	$2, -32(%rbp)
	addq	$2, -8(%rbp)
.L6:
	cmpq	$1, -32(%rbp)
	ja	.L7
	cmpq	$1, -32(%rbp)
	jne	.L8
	movq	-8(%rbp), %rax
	movzbl	(%rax), %eax
	movzbl	%al, %eax
	addl	%eax, -36(%rbp)
.L8:
	movl	-36(%rbp), %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE53:
	.size	__rte_raw_cksum, .-__rte_raw_cksum
	.type	__rte_raw_cksum_reduce, @function
__rte_raw_cksum_reduce:
.LFB54:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -4(%rbp)
	movl	-4(%rbp), %eax
	shrl	$16, %eax
	movl	%eax, %edx
	movl	-4(%rbp), %eax
	movzwl	%ax, %eax
	addl	%edx, %eax
	movl	%eax, -4(%rbp)
	movl	-4(%rbp), %eax
	shrl	$16, %eax
	movl	%eax, %edx
	movl	-4(%rbp), %eax
	movzwl	%ax, %eax
	addl	%edx, %eax
	movl	%eax, -4(%rbp)
	movl	-4(%rbp), %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE54:
	.size	__rte_raw_cksum_reduce, .-__rte_raw_cksum_reduce
	.type	rte_raw_cksum, @function
rte_raw_cksum:
.LFB55:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	-32(%rbp), %rcx
	movq	-24(%rbp), %rax
	movl	$0, %edx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	__rte_raw_cksum
	movl	%eax, -4(%rbp)
	movl	-4(%rbp), %eax
	movl	%eax, %edi
	call	__rte_raw_cksum_reduce
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE55:
	.size	rte_raw_cksum, .-rte_raw_cksum
	.type	rte_ipv4_cksum, @function
rte_ipv4_cksum:
.LFB56:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$24, %rsp
	movq	%rdi, -24(%rbp)
	movq	-24(%rbp), %rax
	movl	$20, %esi
	movq	%rax, %rdi
	call	rte_raw_cksum
	movw	%ax, -2(%rbp)
	cmpw	$-1, -2(%rbp)
	je	.L15
	movzwl	-2(%rbp), %eax
	notl	%eax
	jmp	.L16
.L15:
	movzwl	-2(%rbp), %eax
.L16:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE56:
	.size	rte_ipv4_cksum, .-rte_ipv4_cksum
	.type	rte_ipv4_phdr_cksum, @function
rte_ipv4_phdr_cksum:
.LFB57:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	-24(%rbp), %rax
	movl	12(%rax), %eax
	movl	%eax, -16(%rbp)
	movq	-24(%rbp), %rax
	movl	16(%rax), %eax
	movl	%eax, -12(%rbp)
	movb	$0, -8(%rbp)
	movq	-24(%rbp), %rax
	movzbl	9(%rax), %eax
	movb	%al, -7(%rbp)
	movabsq	$1125899906842624, %rax
	andq	-32(%rbp), %rax
	testq	%rax, %rax
	je	.L19
	movw	$0, -6(%rbp)
	jmp	.L20
.L19:
	movq	-24(%rbp), %rax
	movzwl	2(%rax), %eax
	movzwl	%ax, %eax
	movl	%eax, %edi
	call	ntohs
	subl	$20, %eax
	movzwl	%ax, %eax
	movl	%eax, %edi
	call	htons
	movw	%ax, -6(%rbp)
.L20:
	leaq	-16(%rbp), %rax
	movl	$12, %esi
	movq	%rax, %rdi
	call	rte_raw_cksum
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE57:
	.size	rte_ipv4_phdr_cksum, .-rte_ipv4_phdr_cksum
	.type	rte_ipv4_udptcp_cksum, @function
rte_ipv4_udptcp_cksum:
.LFB58:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	-24(%rbp), %rax
	movzwl	2(%rax), %eax
	movzwl	%ax, %eax
	movl	%eax, %edi
	call	ntohs
	movzwl	%ax, %eax
	subl	$20, %eax
	movl	%eax, -8(%rbp)
	movl	-8(%rbp), %edx
	movq	-32(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	rte_raw_cksum
	movzwl	%ax, %eax
	movl	%eax, -4(%rbp)
	movq	-24(%rbp), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	rte_ipv4_phdr_cksum
	movzwl	%ax, %eax
	addl	%eax, -4(%rbp)
	movl	-4(%rbp), %eax
	shrl	$16, %eax
	movl	%eax, %edx
	movl	-4(%rbp), %eax
	movzwl	%ax, %eax
	addl	%edx, %eax
	movl	%eax, -4(%rbp)
	movl	-4(%rbp), %eax
	notl	%eax
	andl	$65535, %eax
	movl	%eax, -4(%rbp)
	cmpl	$0, -4(%rbp)
	jne	.L23
	movl	$65535, -4(%rbp)
.L23:
	movl	-4(%rbp), %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE58:
	.size	rte_ipv4_udptcp_cksum, .-rte_ipv4_udptcp_cksum
	.section	.rodata
.LC0:
	.string	"can't > packet'len"
	.text
	.globl	send_dns
	.type	send_dns, @function
send_dns:
.LFB59:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -40(%rbp)
	movl	%esi, -44(%rbp)
	movq	$0, -8(%rbp)
	movq	$0, -16(%rbp)
	movq	$0, -24(%rbp)
	movq	$0, -32(%rbp)
	movl	$2048, %edx
	movl	$0, %esi
	movl	$packet, %edi
	call	memset
	movq	$packet, -32(%rbp)
	movq	$packet+14, -16(%rbp)
	movq	$packet+34, -24(%rbp)
	movq	$packet+42, -8(%rbp)
	movq	-32(%rbp), %rax
	movb	$17, 5(%rax)
	movq	-32(%rbp), %rax
	movzbl	5(%rax), %edx
	movq	-32(%rbp), %rax
	movb	%dl, 4(%rax)
	movq	-32(%rbp), %rax
	movzbl	4(%rax), %edx
	movq	-32(%rbp), %rax
	movb	%dl, 3(%rax)
	movq	-32(%rbp), %rax
	movzbl	3(%rax), %edx
	movq	-32(%rbp), %rax
	movb	%dl, 2(%rax)
	movq	-32(%rbp), %rax
	movzbl	2(%rax), %edx
	movq	-32(%rbp), %rax
	movb	%dl, 1(%rax)
	movq	-32(%rbp), %rax
	movzbl	1(%rax), %edx
	movq	-32(%rbp), %rax
	movb	%dl, (%rax)
	movq	-32(%rbp), %rax
	movb	$34, 11(%rax)
	movq	-32(%rbp), %rax
	movzbl	11(%rax), %edx
	movq	-32(%rbp), %rax
	movb	%dl, 10(%rax)
	movq	-32(%rbp), %rax
	movzbl	10(%rax), %edx
	movq	-32(%rbp), %rax
	movb	%dl, 9(%rax)
	movq	-32(%rbp), %rax
	movzbl	9(%rax), %edx
	movq	-32(%rbp), %rax
	movb	%dl, 8(%rax)
	movq	-32(%rbp), %rax
	movzbl	8(%rax), %edx
	movq	-32(%rbp), %rax
	movb	%dl, 7(%rax)
	movq	-32(%rbp), %rax
	movzbl	7(%rax), %edx
	movq	-32(%rbp), %rax
	movb	%dl, 6(%rax)
	movq	-32(%rbp), %rax
	movw	$8, 12(%rax)
	movq	-32(%rbp), %rax
	movw	$8, 12(%rax)
	movq	-16(%rbp), %rax
	movzbl	(%rax), %edx
	andl	$15, %edx
	orl	$64, %edx
	movb	%dl, (%rax)
	movq	-16(%rbp), %rax
	movzbl	(%rax), %edx
	andl	$-16, %edx
	orl	$5, %edx
	movb	%dl, (%rax)
	movq	-16(%rbp), %rax
	movb	$0, 1(%rax)
	movl	-44(%rbp), %eax
	addl	$28, %eax
	movzwl	%ax, %eax
	movl	%eax, %edi
	call	htons
	movq	-16(%rbp), %rdx
	movw	%ax, 2(%rdx)
	movq	-16(%rbp), %rax
	movw	$1, 4(%rax)
	movq	-16(%rbp), %rax
	movw	$0, 6(%rax)
	movq	-16(%rbp), %rax
	movb	$60, 8(%rax)
	movq	-16(%rbp), %rax
	movb	$17, 9(%rax)
	movq	-16(%rbp), %rax
	movq	%rax, %rdi
	call	rte_ipv4_cksum
	movq	-16(%rbp), %rdx
	movw	%ax, 10(%rdx)
	movq	-16(%rbp), %rax
	movl	$16820416, 12(%rax)
	movq	-16(%rbp), %rax
	movl	$16779786, 16(%rax)
	movq	-24(%rbp), %rax
	movw	$13568, (%rax)
	movq	-24(%rbp), %rax
	movw	$23750, 2(%rax)
	movl	-44(%rbp), %eax
	addl	$8, %eax
	movzwl	%ax, %eax
	movl	%eax, %edi
	call	htons
	movq	-24(%rbp), %rdx
	movw	%ax, 4(%rdx)
	movq	-24(%rbp), %rdx
	movq	-16(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	rte_ipv4_udptcp_cksum
	movq	-24(%rbp), %rdx
	movw	%ax, 6(%rdx)
	movl	-44(%rbp), %eax
	addq	$42, %rax
	cmpq	$2048, %rax
	jbe	.L26
	movl	$.LC0, %edi
	call	puts
	movl	$0, %eax
	jmp	.L27
.L26:
	movl	-44(%rbp), %edx
	movq	-40(%rbp), %rcx
	movq	-8(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	memcpy
	movl	-44(%rbp), %eax
	addl	$42, %eax
	movl	%eax, pkt_len(%rip)
	movl	$1, %eax
.L27:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE59:
	.size	send_dns, .-send_dns
	.globl	send_dns3
	.type	send_dns3, @function
send_dns3:
.LFB60:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -40(%rbp)
	movl	%esi, -44(%rbp)
	movq	$0, -8(%rbp)
	movq	$0, -16(%rbp)
	movq	$0, -24(%rbp)
	movq	$0, -32(%rbp)
	movl	$2048, %edx
	movl	$0, %esi
	movl	$packet, %edi
	call	memset
	movq	$packet, -32(%rbp)
	movq	$packet+14, -16(%rbp)
	movq	$packet+34, -24(%rbp)
	movq	$packet+42, -8(%rbp)
	movq	-32(%rbp), %rax
	movb	$-116, (%rax)
	movq	-32(%rbp), %rax
	movb	$-20, 1(%rax)
	movq	-32(%rbp), %rax
	movb	$75, 2(%rax)
	movq	-32(%rbp), %rax
	movb	$92, 3(%rax)
	movq	-32(%rbp), %rax
	movb	$-82, 4(%rax)
	movq	-32(%rbp), %rax
	movb	$-19, 5(%rax)
	movq	-32(%rbp), %rax
	movb	$-28, 6(%rax)
	movq	-32(%rbp), %rax
	movb	$104, 7(%rax)
	movq	-32(%rbp), %rax
	movb	$-93, 8(%rax)
	movq	-32(%rbp), %rax
	movb	$79, 9(%rax)
	movq	-32(%rbp), %rax
	movb	$-14, 10(%rax)
	movq	-32(%rbp), %rax
	movb	$50, 11(%rax)
	movq	-32(%rbp), %rax
	movw	$8, 12(%rax)
	movq	-16(%rbp), %rax
	movzbl	(%rax), %edx
	andl	$15, %edx
	orl	$64, %edx
	movb	%dl, (%rax)
	movq	-16(%rbp), %rax
	movzbl	(%rax), %edx
	andl	$-16, %edx
	orl	$5, %edx
	movb	%dl, (%rax)
	movq	-16(%rbp), %rax
	movb	$0, 1(%rax)
	movl	-44(%rbp), %eax
	addl	$28, %eax
	movzwl	%ax, %eax
	movl	%eax, %edi
	call	htons
	movq	-16(%rbp), %rdx
	movw	%ax, 2(%rdx)
	movq	-16(%rbp), %rax
	movw	$1, 4(%rax)
	movq	-16(%rbp), %rax
	movw	$0, 6(%rax)
	movq	-16(%rbp), %rax
	movb	$60, 8(%rax)
	movq	-16(%rbp), %rax
	movb	$17, 9(%rax)
	movq	-16(%rbp), %rax
	movw	$0, 10(%rax)
	movq	-16(%rbp), %rax
	movl	$503687360, 12(%rax)
	movq	-16(%rbp), %rax
	movl	$-242702144, 16(%rax)
	movq	-24(%rbp), %rax
	movw	$13568, (%rax)
	movq	-24(%rbp), %rax
	movw	$23750, 2(%rax)
	movl	-44(%rbp), %eax
	addl	$8, %eax
	movzwl	%ax, %eax
	movl	%eax, %edi
	call	htons
	movq	-24(%rbp), %rdx
	movw	%ax, 4(%rdx)
	movq	-24(%rbp), %rax
	movw	$0, 6(%rax)
	movl	-44(%rbp), %eax
	addq	$42, %rax
	cmpq	$2048, %rax
	jbe	.L29
	movl	$.LC0, %edi
	call	puts
	movl	$0, %eax
	jmp	.L30
.L29:
	movl	-44(%rbp), %edx
	movq	-40(%rbp), %rcx
	movq	-8(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	memcpy
	movl	-44(%rbp), %eax
	addl	$42, %eax
	movl	%eax, pkt_len(%rip)
	movl	$1, %eax
.L30:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE60:
	.size	send_dns3, .-send_dns3
	.globl	send_dns2
	.type	send_dns2, @function
send_dns2:
.LFB61:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -40(%rbp)
	movl	%esi, -44(%rbp)
	movq	$0, -8(%rbp)
	movq	$0, -16(%rbp)
	movq	$0, -24(%rbp)
	movq	$0, -32(%rbp)
	movl	$2048, %edx
	movl	$0, %esi
	movl	$packet, %edi
	call	memset
	movq	$packet, -32(%rbp)
	movq	$packet+14, -16(%rbp)
	movq	$packet+34, -24(%rbp)
	movq	$packet+42, -8(%rbp)
	movq	-32(%rbp), %rax
	movb	$-116, (%rax)
	movq	-32(%rbp), %rax
	movb	$-20, 1(%rax)
	movq	-32(%rbp), %rax
	movb	$75, 2(%rax)
	movq	-32(%rbp), %rax
	movb	$92, 3(%rax)
	movq	-32(%rbp), %rax
	movb	$-82, 4(%rax)
	movq	-32(%rbp), %rax
	movb	$-19, 5(%rax)
	movq	-32(%rbp), %rax
	movb	$-28, 6(%rax)
	movq	-32(%rbp), %rax
	movb	$104, 7(%rax)
	movq	-32(%rbp), %rax
	movb	$-93, 8(%rax)
	movq	-32(%rbp), %rax
	movb	$79, 9(%rax)
	movq	-32(%rbp), %rax
	movb	$-14, 10(%rax)
	movq	-32(%rbp), %rax
	movb	$50, 11(%rax)
	movq	-32(%rbp), %rax
	movw	$8, 12(%rax)
	movq	-16(%rbp), %rax
	movzbl	(%rax), %edx
	andl	$15, %edx
	orl	$64, %edx
	movb	%dl, (%rax)
	movq	-16(%rbp), %rax
	movzbl	(%rax), %edx
	andl	$-16, %edx
	orl	$5, %edx
	movb	%dl, (%rax)
	movq	-16(%rbp), %rax
	movb	$0, 1(%rax)
	movl	-44(%rbp), %eax
	addl	$28, %eax
	movzwl	%ax, %eax
	movl	%eax, %edi
	call	htons
	movq	-16(%rbp), %rdx
	movw	%ax, 2(%rdx)
	movq	-16(%rbp), %rax
	movw	$1, 4(%rax)
	movq	-16(%rbp), %rax
	movw	$0, 6(%rax)
	movq	-16(%rbp), %rax
	movb	$60, 8(%rax)
	movq	-16(%rbp), %rax
	movb	$17, 9(%rax)
	movq	-16(%rbp), %rax
	movl	$503687360, 12(%rax)
	movq	-16(%rbp), %rax
	movl	$-242702144, 16(%rax)
	movq	-24(%rbp), %rax
	movw	$13568, (%rax)
	movq	-24(%rbp), %rax
	movw	$23750, 2(%rax)
	movl	-44(%rbp), %eax
	addl	$8, %eax
	movzwl	%ax, %eax
	movl	%eax, %edi
	call	htons
	movq	-24(%rbp), %rdx
	movw	%ax, 4(%rdx)
	movq	-24(%rbp), %rdx
	movq	-16(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	rte_ipv4_udptcp_cksum
	movq	-24(%rbp), %rdx
	movw	%ax, 6(%rdx)
	movl	-44(%rbp), %eax
	addq	$42, %rax
	cmpq	$2048, %rax
	jbe	.L32
	movl	$.LC0, %edi
	call	puts
	movl	$0, %eax
	jmp	.L33
.L32:
	movl	-44(%rbp), %edx
	movq	-40(%rbp), %rcx
	movq	-8(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	memcpy
	movl	-44(%rbp), %eax
	addl	$42, %eax
	movl	%eax, pkt_len(%rip)
	movl	$1, %eax
.L33:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE61:
	.size	send_dns2, .-send_dns2
	.section	.rodata
.LC1:
	.string	"."
	.text
	.globl	create_subnet_rr
	.type	create_subnet_rr, @function
create_subnet_rr:
.LFB62:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	call	ldns_rr_new
	movq	%rax, -8(%rbp)
	movl	$8, %esi
	movl	$3, %edi
	call	ldns_native2rdf_int16
	movq	%rax, -16(%rbp)
	movq	-16(%rbp), %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	ldns_rr_push_rdf
	movl	$8, %esi
	movl	$3, %edi
	call	ldns_native2rdf_int16
	movq	%rax, -24(%rbp)
	movq	-24(%rbp), %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	ldns_rr_push_rdf
	movl	$1, %esi
	movl	$3, %edi
	call	ldns_native2rdf_int16
	movq	%rax, -32(%rbp)
	movq	-32(%rbp), %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	ldns_rr_push_rdf
	movl	$32, %esi
	movl	$2, %edi
	call	ldns_native2rdf_int8
	movq	%rax, -40(%rbp)
	movq	-40(%rbp), %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	ldns_rr_push_rdf
	movl	$0, %esi
	movl	$2, %edi
	call	ldns_native2rdf_int8
	movq	%rax, -48(%rbp)
	movq	-48(%rbp), %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	ldns_rr_push_rdf
	movl	ip.21621(%rip), %eax
	leal	1(%rax), %edx
	movl	%edx, ip.21621(%rip)
	movl	%eax, %esi
	movl	$4, %edi
	call	ldns_native2rdf_int32
	movq	%rax, -56(%rbp)
	movq	-56(%rbp), %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	ldns_rr_push_rdf
	movq	-8(%rbp), %rax
	movl	$41, %esi
	movq	%rax, %rdi
	call	ldns_rr_set_type
	movq	-8(%rbp), %rax
	movl	$4096, 28(%rax)
	movq	-8(%rbp), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	ldns_rr_set_ttl
	leaq	-64(%rbp), %rax
	movl	$.LC1, %esi
	movq	%rax, %rdi
	call	ldns_str2rdf_dname
	movq	-64(%rbp), %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	ldns_rr_set_owner
	movq	-8(%rbp), %rax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE62:
	.size	create_subnet_rr, .-create_subnet_rr
	.section	.rodata
.LC2:
	.string	"Error creating ldns_pkt: %s\n"
.LC3:
	.string	"Error creating out_buf: %s\n"
	.text
	.globl	mkdir_dns
	.type	mkdir_dns, @function
mkdir_dns:
.LFB63:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movq	%rdi, -40(%rbp)
	movl	%esi, -44(%rbp)
	movq	%rdx, -56(%rbp)
	movq	%rcx, -64(%rbp)
	movq	$0, -32(%rbp)
	movq	$0, -8(%rbp)
	movl	$0, -12(%rbp)
	movl	-44(%rbp), %eax
	movslq	%eax, %rdx
	movq	-40(%rbp), %rcx
	leaq	-32(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	ldns_wire2pkt
	movl	%eax, -12(%rbp)
	cmpl	$0, -12(%rbp)
	je	.L37
	movl	-12(%rbp), %eax
	movl	%eax, %edi
	call	ldns_get_errorstr_by_id
	movq	%rax, %rsi
	movl	$.LC2, %edi
	movl	$0, %eax
	call	printf
	jmp	.L38
.L37:
	call	create_subnet_rr
	movq	%rax, -24(%rbp)
	movq	-32(%rbp), %rax
	movq	-24(%rbp), %rdx
	movl	$3, %esi
	movq	%rax, %rdi
	call	ldns_pkt_push_rr
	movq	-32(%rbp), %rcx
	movq	-64(%rbp), %rdx
	movq	-56(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	ldns_pkt2wire
	movl	%eax, -12(%rbp)
	cmpl	$0, -12(%rbp)
	je	.L39
	movl	-12(%rbp), %eax
	movl	%eax, %edi
	call	ldns_get_errorstr_by_id
	movq	%rax, %rsi
	movl	$.LC3, %edi
	movl	$0, %eax
	call	printf
	jmp	.L38
.L39:
	movq	-32(%rbp), %rax
	testq	%rax, %rax
	je	.L40
	movq	-32(%rbp), %rax
	movq	%rax, %rdi
	call	ldns_pkt_free
.L40:
	movl	$1, %eax
	jmp	.L44
.L38:
	movq	-32(%rbp), %rax
	testq	%rax, %rax
	je	.L42
	movq	-32(%rbp), %rax
	movq	%rax, %rdi
	call	ldns_pkt_free
	movq	$0, -32(%rbp)
.L42:
	movq	-56(%rbp), %rax
	movq	(%rax), %rax
	testq	%rax, %rax
	je	.L43
	movq	-56(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	free
	movq	-56(%rbp), %rax
	movq	$0, (%rax)
	movq	-56(%rbp), %rax
	movq	$0, (%rax)
.L43:
	movl	$0, %eax
.L44:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE63:
	.size	mkdir_dns, .-mkdir_dns
	.section	.rodata
.LC4:
	.string	"mkdir_dns failed."
.LC5:
	.string	"send_send failed."
	.text
	.globl	handle_ether_packet
	.type	handle_ether_packet, @function
handle_ether_packet:
.LFB64:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$80, %rsp
	movq	%rdi, -72(%rbp)
	movl	$0, -4(%rbp)
	movl	$0, -52(%rbp)
	movw	$0, -18(%rbp)
	movq	$0, -64(%rbp)
	movq	$0, -32(%rbp)
	movq	$0, -16(%rbp)
	movq	$0, -40(%rbp)
	movq	$0, -48(%rbp)
	movq	-72(%rbp), %rax
	movq	%rax, -48(%rbp)
	movq	-48(%rbp), %rax
	movzwl	12(%rax), %eax
	movzwl	%ax, %eax
	movl	%eax, %edi
	call	ntohs
	cmpw	$2048, %ax
	je	.L46
	movq	-48(%rbp), %rax
	movzwl	12(%rax), %eax
	movzwl	%ax, %eax
	movl	%eax, %edi
	call	ntohs
	cmpw	$-31011, %ax
	je	.L46
	movl	$1, %eax
	jmp	.L55
.L46:
	movq	-72(%rbp), %rax
	addq	$14, %rax
	movq	%rax, -32(%rbp)
	movq	-32(%rbp), %rax
	addq	$6, %rax
	movq	%rax, %rdi
	call	ldns_read_uint16
	movw	%ax, -18(%rbp)
	movq	-48(%rbp), %rax
	movzwl	12(%rax), %eax
	movzwl	%ax, %eax
	movl	%eax, %edi
	call	ntohs
	cmpw	$2048, %ax
	jne	.L48
	movzwl	-18(%rbp), %eax
	andl	$8192, %eax
	testl	%eax, %eax
	je	.L49
	movl	$1, %eax
	jmp	.L55
.L49:
	movq	-32(%rbp), %rax
	movzbl	9(%rax), %eax
	cmpb	$17, %al
	je	.L50
	movl	$1, %eax
	jmp	.L55
.L50:
	movq	-32(%rbp), %rax
	movzbl	(%rax), %eax
	andl	$15, %eax
	movzbl	%al, %eax
	sall	$2, %eax
	cltq
	leaq	22(%rax), %rdx
	movq	-72(%rbp), %rax
	addq	%rdx, %rax
	movq	%rax, -16(%rbp)
	movl	32(%rbp), %edx
	movq	-32(%rbp), %rax
	movzbl	(%rax), %eax
	andl	$15, %eax
	movzbl	%al, %ecx
	movl	$0, %eax
	subl	%ecx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	subl	$22, %eax
	movl	%eax, -4(%rbp)
.L48:
	movq	-48(%rbp), %rax
	movzwl	12(%rax), %eax
	movzwl	%ax, %eax
	movl	%eax, %edi
	call	ntohs
	cmpw	$-31011, %ax
	jne	.L51
	movq	-72(%rbp), %rax
	addq	$14, %rax
	movq	%rax, -40(%rbp)
	movq	-40(%rbp), %rax
	movzbl	6(%rax), %eax
	cmpb	$17, %al
	je	.L52
	movl	$1, %eax
	jmp	.L55
.L52:
	movq	-72(%rbp), %rax
	addq	$62, %rax
	movq	%rax, -16(%rbp)
	movl	32(%rbp), %eax
	subl	$62, %eax
	movl	%eax, -4(%rbp)
.L51:
	leaq	-52(%rbp), %rcx
	leaq	-64(%rbp), %rdx
	movl	-4(%rbp), %esi
	movq	-16(%rbp), %rax
	movq	%rax, %rdi
	call	mkdir_dns
	cmpb	$1, %al
	je	.L53
	movl	$.LC4, %edi
	call	puts
	movl	$0, %eax
	jmp	.L55
.L53:
	movl	-52(%rbp), %edx
	movq	-64(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	send_dns
	cmpb	$1, %al
	je	.L54
	movl	$.LC5, %edi
	call	puts
	movl	$0, %eax
	jmp	.L55
.L54:
	movq	-64(%rbp), %rax
	movq	%rax, %rdi
	call	free
	movq	$0, -64(%rbp)
	movl	$1, %eax
.L55:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE64:
	.size	handle_ether_packet, .-handle_ether_packet
	.section	.rodata
.LC6:
	.string	"Create Error"
.LC7:
	.string	"get dev index error:"
.LC8:
	.string	"interface Index:%d\n"
.LC9:
	.string	"sendto n:%d\n"
	.text
	.globl	raw_send
	.type	raw_send, @function
raw_send:
.LFB65:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$104, %rsp
	.cfi_offset 3, -24
	movq	%rdi, -104(%rbp)
	movl	%esi, -108(%rbp)
	movl	$17, %edx
	movl	$3, %esi
	movl	$17, %edi
	call	socket
	movl	%eax, -20(%rbp)
	cmpl	$0, -20(%rbp)
	jns	.L57
	movl	$.LC6, %edi
	call	perror
	movl	$1, %edi
	call	exit
.L57:
	leaq	-48(%rbp), %rax
	movl	$20, %esi
	movq	%rax, %rdi
	call	bzero
	leaq	-96(%rbp), %rax
	movl	$40, %esi
	movq	%rax, %rdi
	call	bzero
	leaq	-96(%rbp), %rax
	movabsq	$7363512080297651813, %rbx
	movq	%rbx, (%rax)
	movw	$51, 8(%rax)
	leaq	-96(%rbp), %rdx
	movl	-20(%rbp), %eax
	movl	$35123, %esi
	movl	%eax, %edi
	movl	$0, %eax
	call	ioctl
	cmpl	$-1, %eax
	jne	.L58
	movl	$.LC7, %edi
	call	perror
	movl	$1, %edi
	call	exit
.L58:
	movl	-80(%rbp), %eax
	movl	%eax, -44(%rbp)
	movl	-80(%rbp), %eax
	movl	%eax, %esi
	movl	$.LC8, %edi
	movl	$0, %eax
	call	printf
	movw	$17, -48(%rbp)
	movl	-108(%rbp), %edx
	leaq	-48(%rbp), %rcx
	movq	-104(%rbp), %rsi
	movl	-20(%rbp), %eax
	movl	$20, %r9d
	movq	%rcx, %r8
	movl	$0, %ecx
	movl	%eax, %edi
	call	sendto
	movl	%eax, -24(%rbp)
	movl	-24(%rbp), %eax
	movl	%eax, %esi
	movl	$.LC9, %edi
	movl	$0, %eax
	call	printf
	movl	-20(%rbp), %eax
	movl	%eax, %edi
	call	close
	movl	$0, %eax
	addq	$104, %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE65:
	.size	raw_send, .-raw_send
	.section	.rodata
.LC10:
	.string	"eno1"
.LC11:
	.string	"./taobao.pcap"
.LC12:
	.string	"error: pcap_open_offline:%s\n"
.LC13:
	.string	"did not capture a packet!"
.LC14:
	.string	"error: pcap_open_live(): %s\n"
.LC15:
	.string	"ret:%d, out_len:%d error:%s\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB66:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$384, %rsp
	movl	$0, -4(%rbp)
	movq	$.LC10, -16(%rbp)
	movq	$.LC11, -24(%rbp)
	movl	$1, -28(%rbp)
	movl	$0, -32(%rbp)
	movl	$2048, -36(%rbp)
	movq	$0, -48(%rbp)
	movq	$0, -56(%rbp)
	movq	$0, -64(%rbp)
	leaq	-352(%rbp), %rdx
	movq	-24(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	pcap_open_offline
	movq	%rax, -56(%rbp)
	cmpq	$0, -56(%rbp)
	jne	.L61
	leaq	-352(%rbp), %rax
	movq	%rax, %rsi
	movl	$.LC12, %edi
	movl	$0, %eax
	call	printf
	movl	$-1, %eax
	jmp	.L65
.L61:
	leaq	-96(%rbp), %rdx
	movq	-56(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	pcap_next
	movq	%rax, -64(%rbp)
	cmpq	$0, -64(%rbp)
	jne	.L63
	movl	$.LC13, %edi
	call	puts
	movl	$-1, %eax
	jmp	.L65
.L63:
	nop
	movq	-64(%rbp), %rax
	movq	-96(%rbp), %rdx
	movq	%rdx, (%rsp)
	movq	-88(%rbp), %rdx
	movq	%rdx, 8(%rsp)
	movq	-80(%rbp), %rdx
	movq	%rdx, 16(%rsp)
	movq	%rax, %rdi
	call	handle_ether_packet
	leaq	-352(%rbp), %rsi
	movl	-32(%rbp), %ecx
	movl	-28(%rbp), %edx
	movl	-36(%rbp), %eax
	movq	%rsi, %r8
	movl	%eax, %esi
	movl	$.LC10, %edi
	call	pcap_open_live
	movq	%rax, -48(%rbp)
	cmpq	$0, -48(%rbp)
	jne	.L64
	leaq	-352(%rbp), %rax
	movq	%rax, %rsi
	movl	$.LC14, %edi
	movl	$0, %eax
	call	printf
	movl	$-1, %eax
	jmp	.L65
.L64:
	movl	pkt_len(%rip), %eax
	movl	%eax, %edx
	movq	-48(%rbp), %rax
	movl	$packet, %esi
	movq	%rax, %rdi
	call	pcap_inject
	movl	%eax, -4(%rbp)
	movl	pkt_len(%rip), %edx
	movl	-4(%rbp), %eax
	movl	%eax, %esi
	movl	$.LC15, %edi
	movl	$0, %eax
	call	printf
	movl	$2, %edi
	call	sleep
	jmp	.L64
.L65:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE66:
	.size	main, .-main
	.local	ip.21621
	.comm	ip.21621,4,4
	.ident	"GCC: (GNU) 4.8.5 20150623 (Red Hat 4.8.5-36)"
	.section	.note.GNU-stack,"",@progbits
