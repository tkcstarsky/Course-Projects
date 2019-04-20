# Principle of Compiler (H) Lab
This is the Principle of Compiler (H) Lab in USTC coded by TKC.
## 目录结构
* c1interpreter 针对C1语言的一个语义分析器
* c1recognizer 针对C1语言的一个语法分析器
* Homework 作业中夹杂的小实验
* README.md 本文件
## 实验说明
* 本实验为编译原理与技术(H)的课程项目，具体实验要求及实验说明文档可查看：   
[https://clarazhang.gitbooks.io/compiler-f2018/content/projects.html](https://clarazhang.gitbooks.io/compiler-f2018/content/projects.html)
* 本项目搭载在ANTLR平台上，目标为编译生成一个自己设计的c1语言编译器。
* 本项目针对C1语言集（一个精简C语言集）完成了一个完成的编译器，其中分别由c1recognizer实现了c1语言的语法分析，c1interpreter实现了c1语言的语意分析并生成了llvm的中间代码，最终由llvm处理并生成二进制可执行程序。