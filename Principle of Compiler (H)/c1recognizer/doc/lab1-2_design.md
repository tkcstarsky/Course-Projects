# Lab1-2 Parser设计方案与分析
## 实验环境
本次实验通过根据C1语言的EBNF描述,完善语法描述文件（.g4文件），使用ANTLR v4自动生成语法分析器，实现对C1语言的语法分析。
## 实验设计
### 1.Token的引用与对Lexer的更新
* 在实验lab1—1中已完成对词法单位的收集和分割，直接生成token字典，这里直接引用C1Lexer中的token字典即可。
* 对于lab1-1中设计的不完善部分，已完成对应部分的更新，即加入了constfloat中几种特殊数字的表示方法。
### 2.规则的条目的选择和逻辑关系
* 规则条目选择：
    * C1语言中的必须规则条目已在原文件中写出，由原文件中的条目即可完成全部C1语言的语法分析。
    * 在原条目上加入一条blockitem，使函数内的结构更清晰。
* 规则间的逻辑关系：
    * 1、最底层的规则即token字典。
    * 2、由不同符号和不同种类数字构成最基础的表达式（exp）和变量（lval）。
    * 3、由exp与符号构成比较表达式（cond）。
    * 4、由exp、lval、cond和各种固有词（if、else、while）构成程序主要执行语句（stmt）。
    * 5、由固有词（const、int、float）、变量和赋值类语句构成变量定义语句（decl）。
    * 6、由执行语句（stmt）与变量定义语句（decl）结合构成函数主体（blockitem）。
    * 7、由Void、函数名与函数主体、大括号（block）构成函数定义（funcdef）。
    * 8、最终由函数定义（funcdef）和变量定义（decl）构成顶层程序定义（compilationUnit）。
### 3.规则条目的具体描述
* 对于每条规则条目的具体描述和Lexer中类似，即由类似正规式的描述方法即可完成。
* 此处列举几个较为特殊的规则条目：
    * exp：exp为最基础的规则，设计中该规则涉及左递归定义，antlr在此处自主完成了左递归定义的实现，故无需做更多修改。
    * stmt：程序的主要语句，具体实现时包括了赋值语句、函数调用语句、函数主体、if-else语句、while循环和分号。
    * decl：decl由两种定义模式组成，分别为带const的静态变量定义和无const的普通变量定义，定义中包含了多种定义方法，即只定义变量、变量数组和定义的同时赋值和多重赋值。
    * funcdef：由于C1语言中只有void类型函数且函数无自变量，故对函数定义的书写较为简单，定义结尾加入block，即把每个函数的申明和具体定义打包一起放在funcdef中。
## 实验分析
* 结合本次实验的测试文件来分析该Parser。
* 对于实验包中已事先提供的四个测试文件，简单测试了所有基本的简单语法，包括了c1语言的大部分语句，调整后均已通过。
* 对于自己的pt正确测试样例，同样使用简单的c1语句进行测试，包括了基本的函数定义和各类语义，也可以通过。
    *下面举例自己的annotation测试结果生成的树：
```c
(compilationUnit (decl (vardecl float (vardef a [ (exp (number 5)) ]) ;)) (compilationUnit (funcdef void main ( ) (block { (blockitem (decl (vardecl int (vardef i = (exp (number 1))) ;))) (blockitem (stmt while ( (cond (exp (lval i)) > (exp (number 0))) ) (stmt (block { (blockitem (stmt if ( (cond (exp (lval i)) < (exp (number 5))) ) (stmt (lvala [ (exp (exp (lval i)) - (exp (number 1))) ]) = (exp (lval i)) ;) else (stmt (lval i) = (exp (number 0)) ;))) (blockitem (stmt (lval i) = (exp (exp (lval i)) + (exp (number 1))) ;)) })))) (blockitem (stmt (lval a [ (exp (number 0)) ]) = (exp (number 0)) ;)) }))))
```
* 对于错误的示例，即ft_var_test.c1，考虑了很多语法可能出错的情况均无问题，故挑选了书写实际有错但被错误读入的几个句子，即在语法分析层次未检测到其定义的问题，实际上可能仍存在问题，超出数组边界、变量未定义等，可见语法分析器也无法处理所有错误。示例见下：
```c
(compilationUnit (funcdef void main ( ) (block { (blockitem (decl (vardecl int (vardef a [ (exp (number 1)) ]) ;)))(blockitem (stmt (lval a [ (exp (lval a [ (exp (lval a [ (exp (lval a [ (exp (number 0)) ])) ])) ])) ]) = (exp (number 1)) ;)) (blockitem (decl (vardecl int (vardef c [ (exp (number 1)) ] = { (exp (number 2)) , (exp (number 3)) , (exp (number 4)) , (exp (number 5)) }) ;))) })))
```
## 实验问题与解决方案
* 大部分设计中的简单问题已在如上的实验设计部分中指出，此处列出的均为一些特别的要点。
* Q1：在最开始定义exp为数字表达式时总是报错。
A：exp中应同时包括数字表达式和字母表达式，故在exp的规则定义中应加上lval。
* Q2：对于compilationUnit的定义存在左递归问题？
A：起初尝试把“compilationUnit？”写在句首，后来发现一只左递归报错，去掉问号可以被自动处理，但由于去掉问号会导致程序必须在2个compilationUnit以上，不符合所有程序的要求。最后将compilationUnit写在（decl|funcdef）后面，解除了左递归问题。
* Q3：文件生成结果中的EOF没有显示？
A：通过助教分享的github进行查询后，在原compilationUnit的定义末尾加上了EOF？，结果无明显变化，但如果改为EOF*,会导致每有一个compilationUnit就多匹配一个EOF在其末尾，感觉也不合理，故最后保留了EOF？的做法。