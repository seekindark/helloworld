	.file	"test.c"
	.text
	.globl	testfunc
	.def	testfunc;	.scl	2;	.type	32;	.endef
	.seh_proc	testfunc
testfunc:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	.seh_endprologue
	movl	%ecx, 16(%rbp)
	movl	%edx, %eax
	movb	%al, 24(%rbp)
	addl	$4, 16(%rbp)
	movzbl	24(%rbp), %eax
	addl	$1, %eax
	movb	%al, 24(%rbp)
	movl	$0, %eax
	popq	%rbp
	ret
	.seh_endproc
	.def	__main;	.scl	2;	.type	32;	.endef
	.globl	main
	.def	main;	.scl	2;	.type	32;	.endef
	.seh_proc	main
main:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$48, %rsp
	.seh_stackalloc	48
	.seh_endprologue
	call	__main
	movl	$0, -4(%rbp)
	movb	$99, -5(%rbp)
	movsbl	-5(%rbp), %edx
	movl	-4(%rbp), %eax
	movl	%eax, %ecx
	call	testfunc
	addq	$48, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.ident	"GCC: (GNU) 7.4.0"
