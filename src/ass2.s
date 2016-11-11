	.file	"ass2.c"
	.text
	.globl	atrshmlog_get_tsc_x86_64_gnu
	.type	atrshmlog_get_tsc_x86_64_gnu, @function
atrshmlog_get_tsc_x86_64_gnu:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
#APP
# 16 "ass2.c" 1
	rdtsc
# 0 "" 2
#NO_APP
	movl	%eax, -4(%rbp)
	movl	%edx, -8(%rbp)
	movl	-8(%rbp), %eax
	salq	$32, %rax
	movq	%rax, %rdx
	movl	-4(%rbp), %eax
	orq	%rdx, %rax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	atrshmlog_get_tsc_x86_64_gnu, .-atrshmlog_get_tsc_x86_64_gnu
	.ident	"GCC: (GNU) 5.3.1 20160406 (Red Hat 5.3.1-6)"
	.section	.note.GNU-stack,"",@progbits
