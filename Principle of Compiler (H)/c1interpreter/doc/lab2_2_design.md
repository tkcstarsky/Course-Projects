# Lab2-2 C1 语言的 LLVM IR 代码生成器
## 实验环境
本次实验通过编写src目录assembly_builder.cpp文件，使得antlr4通过与lab1-3生成的语法AST结合自动生成中间代码llvm ir，并由llvm编译后执行变成可执行程序。
## 实验设计
### 1.Visitor Pattern
* 本次实验中仍主要使用Visitor Pattern（访问者模式），与lab1-3中类似。
### 2.实现各个语法规则条目的具体分析函数
* 根据不同文法的需要，共需实现14个中间代码生成函数。
* 未完成（continuing……）
