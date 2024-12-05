# 中国矿业大学编译技术实验一

一个 C 语言的词法分析器

# 实现的功能

## 1. 关键字识别

支持的 C 语言关键字：

```c
if, else, while, do, main, int, float, double, return, const, void,
continue, break, char, unsigned, enum, long, switch, case, auto,
static, for, default, typedef, struct, union, sizeof, goto, extern,
register, volatile, signed, short
```

## 2. 数字字面量

### 2.1 整数字面量

- 十进制整数：`123`, `42`, `0`
- 十六进制整数：`0x1A`, `0XFF`
- 二进制整数：`0b101`, `0B1100`
- 八进制整数：`017`, `0777`

### 2.2 浮点数字面量

- 普通浮点数：`3.14`, `0.5`, `42.0`
- 科学计数法：`1.23e-4`, `3E10`
- 支持小数点前后数字可选：`.5`, `5.`

## 3. 字符和字符串

### 3.1 字符字面量

- 普通字符：`'a'`, `'1'`, `'+'`
- 转义字符：
  - `'\n'` - 换行
  - `'\t'` - 制表符
  - `'\r'` - 回车
  - `'\\'` - 反斜杠
  - `'\''` - 单引号
  - `'\"'` - 双引号
  - `'\0'` - 空字符

### 3.2 字符串字面量

- 普通字符串：`"Hello, World!"`
- 包含转义字符的字符串：`"Hello\nWorld"`
- 空字符串：`""`

## 4. 运算符和分隔符

### 4.1 算术运算符

- `+`, `-`, `*`, `/`, `%`
- `++`, `--`

### 4.2 关系运算符

- `==`, `!=`
- `<`, `<=`, `>`, `>=`

### 4.3 逻辑运算符

- `&&`, `||`, `!`

### 4.4 位运算符

- `&`, `|`, `^`, `~`
- `<<`, `>>`

### 4.5 赋值运算符

- 简单赋值：`=`
- 复合赋值：`+=`, `-=`, `*=`, `/=`, `%=`
- 位运算赋值：`&=`, `|=`, `^=`, `<<=`, `>>=`

### 4.6 其他运算符

- 条件运算符：`?`, `:`
- 成员访问：`.`, `->`
- 取地址：`&`
- 解引用：`*`
- sizeof 运算符
- 逗号运算符：`,`

### 4.7 分隔符

- 括号：`()`, `[]`, `{}`
- 分号：`;`
- 逗号：`,`
- 预处理符号：`#`

## 5. 注释处理

### 5.1 单行注释

- 以 `//` 开始，到行尾结束

### 5.2 多行注释

- 以 `{*` 开始，以 `*}` 结束
- 支持嵌套注释
- 跨行处理
- 未闭合检测

## 6. 空白字符处理

### 6.1 基本空白字符

- 空格：` `
- 制表符：`\t`
- 回车：`\r`
- 换行：`\n`

### 6.2 行列追踪

- 维护当前行号
- 维护当前列号
- 支持不同系统的换行符（`\n`, `\r\n`）

## 7. 错误处理

### 7.1 词法错误检测

- 非法标识符（数字开头）
- 未闭合的字符字面量
- 未闭合的字符串字面量
- 未闭合的注释
- 非法的数字格式（如 `0x` 后无数字）
- 非法的字符字面量（空字符字面量）

### 7.2 错误信息

- 包含错误类型
- 提供行号信息
- 提供列号信息
- 显示具体的错误内容
