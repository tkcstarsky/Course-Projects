# Homework 10 (11.26)
## 一、书上习题6.18（改动）
* a)sizeof(a)的值为0，因为由于其a[0][4]并未在汇编代码中给a分配知实际的空间，因此最后输出的a的大小为0。
* b)实际运行中a[0][0]的值即应该之前定义的变量的大小，但由于i和j均比a的大小大，故之前分配的应为j变量，故值为8。
* c)汇编码中这些调试信息的含义：
```mips
main:
.LFB0:
        .cfi_startproc                          ;程序开始
        leal    4(%esp), %ecx
        .cfi_def_cfa 1, 0                       ;第1号寄存器所指位置加0字节
        andl    $-16, %esp
        pushl   -4(%ecx)
        pushl   %ebp
        .cfi_escape 0x10,0x5,0x2,0x75,0         ;向展开信息添加0x10,0x5,0x2,0x75,0（未完全弄懂）
        movl    %esp, %ebp
        pushl   %ecx
        .cfi_escape 0xf,0x3,0x75,0x7c,0x6       ;;向展开信息添加0xf,0x3,0x75,0x7c,0x6
        subl    $20, %esp
        movl    %gs:20, %eax
        movl    %eax, -12(%ebp)
        xorl    %eax, %eax
        movl    $4, -24(%ebp)
        movl    $8, -20(%ebp)
        movl    -16(%ebp), %eax
        subl    $4, %esp
        pushl   %eax
        pushl   $0
        pushl   $.LC0
        call    printf
        addl    $16, %esp
        movl    $0, %eax
        movl    -12(%ebp), %edx
        xorl    %gs:20, %edx
        je      .L3
        call    __stack_chk_fail
.L3:
        movl    -4(%ebp), %ecx
        .cfi_def_cfa 1, 0                       ;第1号寄存器所指位置加0字节
        leave
        .cfi_restore 5                          ;第5号寄存器恢复到函数开始前状态
        leal    -4(%ecx), %esp
        .cfi_def_cfa 4, 4                       ;第4号寄存器(esp)所指位置加4字节
        ret
        .cfi_endproc                            ;程序结束
```

## 二、自主命题 
### 1、环境定义
* 机器配置：MacBook Pro (13-inch, 2017, Four Thunderbolt 3 Ports)
    * CPU：3.1 GHz Intel Core i5
    * 内存：8 GB 2133 MHz LPDDR3
* 操作系统：macOS Mojave v10.14.1
* 编译器：apple LLVM version 9.1.0 (clang-902.0.39.2)
* 源代码test.c代码如下：
```c
#include <stdio.h>
int main(){
    long a[1][1];
    int i;
    int b[1][2];
    printf("a:%08lx\n", a);
    printf("b:%08lx\n", b);
    printf("i:%08lx\n", &i);
    return 0;
}

```
* 编译选项：
```bash
gcc test.c -o test -w -O1
```
​	得到运行结果：

```
a:7ffee17618f0
b:7ffee17618e8
i:7ffee17618e4
```
### Q1：(局部数据存储顺序)
* 请分析i的地址小于a、b的原因，并判断将a数组的声明改为
```c
	long a[1][2];
```
​后的运行结果（假设存储地址没有随机化）？

* A1：分配局部数据时，先分配小的，再分配大的，大小相等的，按照先后顺序分配。   
改变后的运行结果：
```
a:7ffee02c88e0
b:7ffee02c88d8
i:7ffee02c88d4
```

### Q2：(数据存储中的对齐问题)
* 观察上题结果中b-i的结果可知int占了4字节，若将b数组改为：
```c
	short b[1][1];
```
​后的运行结果为：
```
a:7ffeea1ac8f0
b:7ffeea1ac8ee
i:7ffeea1ac8e8
```
可见b-i的结果变成了6字节，原因为？若将i也改为：
```c
	short i;
```
则输出结果应为多少？

* A2：由于分配空间时应以4字节对齐，故从a往b和i分配时，b仅为2字节，故i为了对齐会寻找下一个4字节的开头即e8。  
更改后i也变成2字节，正好和b连在一起成为4字节对齐，故又成为连续的地址，结果应为：
```
a:7ffeef12a8f0
b:7ffeef12a8ee
i:7ffeef12a8ec
```