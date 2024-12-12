# LiteC Parser Implementation Overview

## 1. 解析器结构

LiteC 解析器采用递归下降解析（Recursive Descent Parsing）技术，主要由以下几个部分组成：

- **Parser 类**：负责解析输入的源代码，维护当前的 token 状态。
- **AST 节点**：抽象语法树（AST）的节点类型，用于表示不同的语法结构。

## 2. 主要解析函数

### 2.1 [statement()](cci:1://file:///c:/Users/ahuma/Desktop/litec/parser.cpp:102:0-177:1)

- **功能**：解析一条语句。
- **实现思路**：
  - 使用[match()](cci:1://file:///c:/Users/ahuma/Desktop/litec/parser.cpp:15:0-21:1)函数检查当前 token 是否匹配特定语法结构（如`if`、`while`、`do`等）。
  - 根据匹配的 token 类型调用相应的解析函数（如`if`语句解析调用[boolExpr()](cci:1://file:///c:/Users/ahuma/Desktop/litec/parser.cpp:244:0-261:1)）。
  - 处理赋值语句和表达式语句。
  - 在遇到语法错误时，调用[synchronize()](cci:1://file:///c:/Users/ahuma/Desktop/litec/parser.cpp:52:0-72:1)进行错误恢复。

### 2.2 [block()](cci:1://file:///c:/Users/ahuma/Desktop/litec/parser.cpp:86:0-103:1)

- **功能**：解析一个代码块。
- **实现思路**：
  - 解析左大括号`{`后，循环调用[statement()](cci:1://file:///c:/Users/ahuma/Desktop/litec/parser.cpp:102:0-177:1)以解析块内的多条语句。
  - 使用[consume()](cci:1://file:///c:/Users/ahuma/Desktop/litec/parser.cpp:23:0-29:1)确保块以右大括号`}`结束。
  - 处理可能的语法错误，确保在错误发生时能够恢复解析。

### 2.3 [boolExpr()](cci:1://file:///c:/Users/ahuma/Desktop/litec/parser.cpp:244:0-261:1)

- **功能**：解析布尔表达式。
- **实现思路**：
  - 调用[expression()](cci:1://file:///c:/Users/ahuma/Desktop/litec/parser.cpp:179:0-194:1)解析左侧表达式。
  - 检查当前 token 是否为比较运算符（如`<`、`<=`、`>`、`>=`等），并解析右侧表达式。
  - 构造`BinaryExpr`节点以表示布尔表达式。

### 2.4 [expression()](cci:1://file:///c:/Users/ahuma/Desktop/litec/parser.cpp:179:0-194:1)

- **功能**：解析算术表达式。
- **实现思路**：
  - 调用[term()](cci:1://file:///c:/Users/ahuma/Desktop/litec/parser.cpp:196:0-211:1)解析初始表达式。
  - 处理加法和减法运算符，通过循环调用[term()](cci:1://file:///c:/Users/ahuma/Desktop/litec/parser.cpp:196:0-211:1)来解析更复杂的表达式。
  - 构造`BinaryExpr`节点以表示算术表达式。

### 2.5 [term()](cci:1://file:///c:/Users/ahuma/Desktop/litec/parser.cpp:196:0-211:1)

- **功能**：解析乘法和除法运算。
- **实现思路**：
  - 调用[factor()](cci:1://file:///c:/Users/ahuma/Desktop/litec/parser.cpp:213:0-234:1)解析基本因子。
  - 处理乘法和除法运算符，通过循环调用[factor()](cci:1://file:///c:/Users/ahuma/Desktop/litec/parser.cpp:213:0-234:1)来解析更复杂的表达式。
  - 构造`BinaryExpr`节点以表示乘法和除法表达式。

### 2.6 [factor()](cci:1://file:///c:/Users/ahuma/Desktop/litec/parser.cpp:213:0-234:1)

- **功能**：解析单个因子（如数字、标识符或括号内的表达式）。
- **实现思路**：
  - 检查当前 token 类型，支持括号表达式、标识符和字面量（如整数、浮点数）。
  - 通过返回相应的 AST 节点（如`IdentifierExpr`或`LiteralExpr`）来构造因子。

## 3. 错误处理

- **功能**：处理解析过程中的错误。
- **实现思路**：
  - 使用`try-catch`块捕获`ParseError`异常。
  - 在发生错误时调用[synchronize()](cci:1://file:///c:/Users/ahuma/Desktop/litec/parser.cpp:52:0-72:1)方法，跳过到下一个可恢复的状态。
  - 通过详细的错误消息提供反馈，帮助开发者调试。

## 4. 总结

LiteC 解析器通过一系列递归函数实现了对简化 C 语言的语法解析。每个解析函数负责特定的语法结构，并通过构建 AST 节点来表示解析结果。错误处理机制确保在遇到语法错误时能够恢复解析，提升了解析器的健壮性和可用性。
