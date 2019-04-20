# GCC的配置和使用测试
## 测试内容介绍
本次测试源文件选择了简单的a+b计算c语言程序，用来分析具体源码的内容和测试各种不同的gcc指令。源文件即为根目录下的adder.c，再使用make编译后会依次出现对应不同阶段的汇编程序，也位于根目录下。
## 不同的编译指令测试
对于gcc后面所添加的不同指示符，分别有：   
-E：编译产生预处理文件(.i文件)   
具体来说就是对原c语言中各种预先设置好的内容进行插入(如include,define和条件编译)。在本次的测试程序中即为对stdio.h中内容的展开，去adder.i文件中的前面绝大部分内容。   
-S：编译产生汇编文件(.s文件)   
-c：编译产生目标文件(.o文件)   
-o：编译产生可执行文件   
-m32：产生的汇编语言为32位机器语言   
-m64：产生的汇编语言为64位机器语言   
## 汇编文件分析
### 32位汇编程序分析
```mips
	.section	__TEXT,__text,regular,pure_instructions   
	.macosx_version_min 10, 13   
	.globl	_main                ## -- Begin function main   
	.p2align	4, 0x90   
_main:                                  ## 主函数开始   
	.cfi_startproc   
	pushl	%ebp   
Lcfi0: ##设置环境   
	.cfi_def_cfa_offset 8   
Lcfi1:   
	.cfi_offset %ebp, -8   
	movl	%esp, %ebp   
Lcfi2:   
	.cfi_def_cfa_register %ebp   
	subl	$24, %esp   
	calll	L0$pb   
L0$pb:   
	popl	%eax   
	leal	L_.str-L0$pb(%eax), %eax   
	movl	$1, -4(%ebp) ##完成对a的赋值   
	movl	$2, -8(%ebp) ##完成对b的赋值   
	movl	-4(%ebp), %ecx   
	addl	-8(%ebp), %ecx ##完成a+b   
	movl	%ecx, -12(%ebp)   
	movl	-12(%ebp), %ecx   
	movl	%eax, (%esp)   
	movl	%ecx, 4(%esp) ##完成对c的赋值   
	calll	_printf ##调用printf函数输出   
	xorl	%ecx, %ecx   
	movl	%eax, -16(%ebp)         ## 4-byte Spill   
	movl	%ecx, %eax   
	addl	$24, %esp   
	popl	%ebp   
	retl   
	.cfi_endproc   
                                        ## -- End function   
	.section	__TEXT,__cstring,cstring_literals   
L_.str:                                 ## @.str   
	.asciz	"c=%d\n" ##输出的字符串内容   


.subsections_via_symbols   
```
### 64位汇编程序分析
对于64位和32位汇编程序的对比分析，容易看出其大体上区别不大，主要区别还是在于内存中取数的存取步长的变化，64位大体上的数值均为32位的两倍，在其余计算和系统调用方面均无区别（由于与32位汇编程序较相似故不再注释）。   
```mips
	.section	__TEXT,__text,regular,pure_instructions   
	.macosx_version_min 10, 13   
	.globl	_main                   ## -- Begin function main   
	.p2align	4, 0x90   
_main:                                  ## @main   
	.cfi_startproc   
	pushq	%rbp   
Lcfi0:   
	.cfi_def_cfa_offset 16   
Lcfi1:   
	.cfi_offset %rbp, -16   
	movq	%rsp, %rbp   
Lcfi2:   
	.cfi_def_cfa_register %rbp   
	subq	$16, %rsp   
	leaq	L_.str(%rip), %rdi   
	movl	$1, -4(%rbp)   
	movl	$2, -8(%rbp)   
	movl	-4(%rbp), %eax   
	addl	-8(%rbp), %eax   
	movl	%eax, -12(%rbp)   
	movl	-12(%rbp), %esi   
	movb	$0, %al   
	callq	_printf   
	xorl	%esi, %esi   
	movl	%eax, -16(%rbp)         ## 4-byte Spill   
	movl	%esi, %eax   
	addq	$16, %rsp   
	popq	%rbp   
	retq   
	.cfi_endproc   
                                        ## -- End function   
	.section	__TEXT,__cstring,cstring_literals   
L_.str:                                 ## @.str   
	.asciz	"c=%d\n"   


.subsections_via_symbols   

```