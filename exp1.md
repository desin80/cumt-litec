﻿**《编译技术》课程实验指导书**

**实验一    词法分析**

1. 实验目的
   1. 学会针对DFA转换图实现相应的高级语言源程序。
   1. 深刻领会状态转换图的含义，逐步理解有限自动机。
   1. 掌握手工生成词法分析器的方法，了解词法分析器的内部工作原理。
1. 实验内容

C计算机语言的编译程序的词法分析部分实现。

从左到右扫描每行该语言源程序的符号，拼成单词，换成统一的内部表示（token）送给语法分析程序。

为了简化程序的编写，有具体的要求如下：

1. 空白符仅仅是空格、回车符、制表符。
1. 代码是自由格式。
1. 注释应放在花括号之内，并且不允许嵌套

**C语言的单词**

|**保留字**|**特殊符号**|**其他**|
| :-: | :-: | :-: |
|if|+|<p>**标识符**</p><p>（首符号为字母或下划线，其他由一个或更多的字母或数字或下划线构成）</p>|
|else|-||
|while|\*||
|do|/||
|main|=|<p>**数**</p><p>数字的属性包括三种：类型属性（整型、浮点型等）、进制属性（十进制、八进制、十六进制）和后缀属性（如短类型、宽类型等辅助类型信息）</p><p>如：123L</p>|
|int|<||
|float|{||
|double|}||
|return|;||
|const|（||
|void|）|<p>**字符常量**</p><p>源程序中任意字符，除了’、backslash\、newline</p><p>字符常量如：‘a’</p><p></p><p>**字符串常量**</p><p>源程序中任意字符串，除了”、backslash\、newline </p><p>字符串常量如：“a”</p>|
|continue|‘||
|break|‘||
|char|“||
|unsigned|“||
|enum|==||
|long|!=||
|switch|&&||
|case|||||
|unsigned|>||
|auto|>=||
|static|<=||

1. 实验要求

要求实现编译器的以下功能：

1. 按规则拼单词,并转换成二元式形式
1. 删除注释行
1. 删除空白符 (空格、回车符、制表符)
1. 显示源程序，在每行的前面加上行号，并且打印出每行包含的记号的二元形式
1. 发现并定位错误。

词法分析进行具体的要求：

1) 详细给出标识符、数字、字符和字符串的状态转换图
1) 画出空白间隔符的状态转换图、换行符的状态装换图、注释（//、/\*  \*/）的状态转换图。

(3) 词法分析的具体功能实现是一个函数GetToken()，每次调用都对剩余的字符串分析得到一个单词或记号识别其种类，收集该记号的符号串属性，当识别一个单词完毕，采用返回值的形式返回符号的种类，同时采用程序变量的形式提供当前识别出记号的属性值。

(4) 标识符和保留字的词法构成相同，为了更好的实现，把语言的保留字建立一个表格存储，这样可以把保留字的识别放在标示符之后，用识别出的标示符对比该表格，如果存在该表格中则是保留字，否则是一般标识符。




