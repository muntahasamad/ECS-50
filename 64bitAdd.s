.data
num1:
	.long 1
	.long 1
num2:
	.long 0
	.long 0
.text
.globl _start
_start:
	movl $0, %edx
	movl $num1, %ecx
	addl $4, %ecx
	movl (%ecx), %eax
	movl $num2, %ecx
	addl $4, %ecx
	addl (%ecx), %eax
	jnc no_carry
	incl %edx
no_carry:
	movl $num1, %ecx
	addl (%ecx), %edx
	movl $num2, %ecx
	addl (%ecx), %edx
done: 
	movl $0, %ecx
