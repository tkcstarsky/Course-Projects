# Lab1-3 AST生成器设计方案与分析
## 实验环境
本次实验通过修改完善src目录下的recognizer.cpp和syntax_tree_builder.cpp文件，使得antlr4通过与C1Parser和C1Lexer结合自动生成二进制AST生成器。
## 实验设计
### 1.Visitor Pattern
* 本次实验中主要使用Visitor Pattern（访问者模式），通过维护与调用各个词法的接口来实现对文法的分析与AST分析树的生成。
* 基类C1ParserBaseVisitor是ANTLR v4为类C1ParserVisitor提供的一个空的实现，因此本次实验中可以从它派生c1语言需要的访问者类，来根据需要实现其中的部分或全部方法。
### 2.实现各个语法规则条目的具体分析函数
* 由C1Parser中对应的语法规则，共得到13个需要分析的语法分析函数。
* 对不同的语法分析函数，主要需要对照语法文件，逐步完成从左往右的匹配分析，通过ctx对语法分析结果的分支判断，可得知当前语句选择的语法项，进而进入该项通过调用不同语法/词法项的visit函数实现AST分析树。
* 对于不同语法项需要的数据结构均已在include/syntax_tree.h中给出，故实际实现时只需对应其中的结构并合理对结构中的不同变量找到对应的取值即可。
* 本次实验的主要难点在于分析不同语法项的方法不尽相同，13个函数的整体区别较大，相似点主要还是在visit函数的调用上。
### 3.对主文件recognizer.cpp的微调
* 本点较为简单，只需要按照实验要求将44改为以下即可：
```cpp
auto tree = parser.compilationUnit();
```
## 实验分析
* 本次实验中最终测试结果表明大部分语句均已实现AST分析树的成功生成，结果与实验git库提供的二进制文件相同。
* 由于时间有限仍留有bug语句：具体为当赋值语句的右边也为一表达式时：例如i=i+1；   
会出现分析失败的情况，具体的错误分析见下方实验问题部分。
* 对于实验中提供的一系列报错函数均未使用，实际可在各个节点加入以直接报出当前读入文件隐藏的语法的错误或警告。
## 实验问题与解决方案
* Q1：C1Parser中的语法规则是否要与本次实验cpp文件中的函数一一对应？
A：经过长时间的尝试与失败后发现必须保持两者相同才可以完成分析，即每个语法规则都要有对应的语法分析函数。因此在本次实验中去除了之前C1Parser中不必要的语法规则，合并了语法规则，使其与实验模板中的13个函数恰好一一对应。
* Q2：对于指针转换的一系列问题
A：对于智能指针使用时存在了一系列的问题，明明应该正确的指针返回值却始终导致中断错误，对于visitLval实现时：   
```cpp
auto result = new lval_syntax;
    result->line = ctx->getStart()->getLine();
    result->pos = ctx->getStart()->getCharPositionInLine();
    result->name = ctx->Identifier()->getSymbol()->getText();

    if(ctx->LeftBracket()){
        auto expressions = ctx->exp();
        result->array_index.reset(visit(expressions).as<expr_syntax *>());
    }
    else{
        result->array_index.reset(static_cast<expr_syntax *>(nullptr));
    }
    return dynamic_cast<lval_syntax *>(result);
```
上述代码段中最终返回值应该不会出错，但实际出错语句在return上，即时调整智能指针仍存在问题，因此在分析“i=i+1”时始终存在错误，希望后续能处理该问题。