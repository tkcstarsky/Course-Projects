# Homework 9
## 7.9
在该汇编代码中有关的指令后加注释，将源程序中的操作和生成的汇编代码对应起来，以判断确实是用短路计算方式来完成布尔表达式计算的。
```c
void main() {
	int i,j;
	while ( (i || j) && (j > 5) ) {
		i = j;
	}
}
```
```mips
.file   "ex7-9.c" ;
        .text
        .globl  main
        .type   main, @function         ; void main()
main:
.LFB0:                                  ; int i,j;
        pushl   %ebp
        movl    %esp, %ebp
        subl    $16, %esp
        jmp     .L2
.L5:                                    ; i=j
        movl    -4(%ebp), %eax          ; i=%eax
        movl    %eax, -8(%ebp)          ; %eax=j
.L2:                                    ; while((i||j)) (while条件的前一半)
        cmpl    $0, -8(%ebp)            ; if(i!=0) jump to L3
        jne     .L3
        cmpl    $0, -4(%ebp)            ; if(j!=0) jump to L3
        jne     .L3
        cmpl    $0, -4(%ebp)            ; if(j==0) jump to L4
        je      .L4
.L3:                                    ; while(j>5) (while条件的后一半)
        cmpl    $5, -4(%ebp)            ; if(j>5) jump to L5
        jg      .L5
.L4:                                    ; while end (跳出while循环)
        movl    $0, %eax
        leave
        ret
.LFE0:
        .size   main, .-main
        .ident  "GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.5) 5.4.0 20160609"
```
## 7.10
* (a)由于在编译时对每个控制语句均生成一个label，故同时出现多个label的原因可能是在同一位置可能会有多个其他位置的控制语句转跳到该处，如L2为if语句判断为错进else分支时的转跳的label，而L4为while循环转跳回来的label，故同一位置生成了两个label。同理L1、L3、L5也同样。
* (b).L1的标号可能是固定代表的函数返回值的label，即在该label后固定进行函数的结束和返回值操作，没有引用该label的地方是因为该程序的main函数无返回值且为线性结构，中途没有出现return，故在结尾自动执行.L1的内容即可，无需再别处引用。
* (c)L3~L5分别处理了以下语句：
    * L5对j变量进行了+1操作，即原程序中的j++。
    * L4即为while循环判断语句，相当于原程序中的饿while(i)。
    * L3设定返回值为0并结束该函数。    
* 没有L1与L2可能是在汇编最后将L1、L2与其他的label合并了，就像第(a)问中的一样，本来用于返回值的L1与循环结束的L3合并，本来用于else的L2与循环while的转跳L4合并，即得到现在的函数。

##7.17
* (a)由于每一维下界都为0，故对于不同维度的情况应为：(假设第i维的大小为ni)
    * 1维：a[i]=i*w+base
    * 2维：a[i][j]=((i*n2)+j)*w+base
    * n维：a[i1,i2,..,ik]=((...((i1*n2+i2)*n3+i3)...)*nk+ik)*w+base
* (b):
    * L-->id        {L.place=id.place;L.offset=null;}
    * L-->L1[E]     {L.place=newTemp();
                    L.offset=newTemp();
                    L.ndim=L1.ndim+1;
                    } 