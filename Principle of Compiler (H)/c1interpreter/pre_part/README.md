# Lab2-1 LLVM IR代码的生成
## 文档结构
* fib.ll-----------手动翻译的LLVM IR文档
* fib_gen.cpp------自动生成LLVM IR的源码
* Makefile---------make文件
* display.sh-------用于判断生成的源码正确性的脚本文件（Makefile会自动调用）
## Makefile说明
* 共选取了4个config以完成编译：
    * --cxxflags：c++代码的llvmIR代码生成
    * --ldflags：LLVM IR的打印函数link
    * --system-libs：系统自带库文件的链接引入
    * --libs：LLVM IR库文件的链接引入
* Makefile在执行完编译后自动运行display.sh脚本
## display.sh脚本说明
* 自动运行make生成的二进制文件生成fib_gen.ll
* 打开fib_gen.ll文件
* 使用lli执行fib_gen.ll，并给出echo $?的结果
## 实验分析
* 实验中手动翻译代码时由于为了美观清晰直接使用了%ret类似的字母变量名，而未使用数字%1类似的变量名，故源码fib.ll与原来的cpp代码映射关系明确，直接在注释中即可看出。
* 为使BB模块的结构清晰，没有使用默认的label名而自己命名。