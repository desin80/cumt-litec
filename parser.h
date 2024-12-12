#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <string>
#include <stdexcept>
#include <memory>
#include <iostream>

// 语法错误异常类
class ParseError : public std::runtime_error {
public:
    ParseError(const std::string& message) : std::runtime_error(message) {}
};

// 抽象语法树节点基类
class ASTNode {
public:
    virtual ~ASTNode() = default;
    int line;    // 源代码行号
    int column;  // 源代码列号
    
    // 添加虚函数用于打印
    virtual void print(int indent = 0) const = 0;
};

// 表达式节点基类
class Expr : public ASTNode {
public:
    virtual ~Expr() = default;
    virtual void print(int indent = 0) const override = 0;
};

// 语句节点基类
class Stmt : public ASTNode {
public:
    virtual ~Stmt() = default;
    virtual void print(int indent = 0) const override = 0;
};

// 二元表达式节点
class BinaryExpr : public Expr {
public:
    std::unique_ptr<Expr> left;
    TokenType op;
    std::unique_ptr<Expr> right;

    BinaryExpr(std::unique_ptr<Expr> left, TokenType op, std::unique_ptr<Expr> right)
        : left(std::move(left)), op(op), right(std::move(right)) {}
        
    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "BinaryExpr(" << Lexer::getTokenName(op) << ")\n";
        left->print(indent + 2);
        right->print(indent + 2);
    }
};

// 字面量表达式节点
class LiteralExpr : public Expr {
public:
    Token value;

    explicit LiteralExpr(Token value) : value(value) {}
    
    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "Literal(" << value.lexeme << ")\n";
    }
};

// 标识符表达式节点
class IdentifierExpr : public Expr {
public:
    Token name;

    explicit IdentifierExpr(Token name) : name(name) {}
    
    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "Identifier(" << name.lexeme << ")\n";
    }
};

// 块语句节点
class BlockStmt : public Stmt {
public:
    std::vector<std::unique_ptr<Stmt>> statements;

    explicit BlockStmt(std::vector<std::unique_ptr<Stmt>> statements)
        : statements(std::move(statements)) {}
        
    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "Block {\n";
        for (const auto& stmt : statements) {
            if (stmt) stmt->print(indent + 2);
        }
        std::cout << std::string(indent, ' ') << "}\n";
    }
};

// 表达式语句节点
class ExpressionStmt : public Stmt {
public:
    std::unique_ptr<Expr> expression;

    explicit ExpressionStmt(std::unique_ptr<Expr> expression)
        : expression(std::move(expression)) {}
        
    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "ExprStmt:\n";
        expression->print(indent + 2);
    }
};

// if语句节点
class IfStmt : public Stmt {
public:
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> thenBranch;
    std::unique_ptr<Stmt> elseBranch;  // 可能为nullptr

    IfStmt(std::unique_ptr<Expr> condition,
           std::unique_ptr<Stmt> thenBranch,
           std::unique_ptr<Stmt> elseBranch = nullptr)
        : condition(std::move(condition))
        , thenBranch(std::move(thenBranch))
        , elseBranch(std::move(elseBranch)) {}
        
    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "If:\n";
        std::cout << std::string(indent + 2, ' ') << "Condition:\n";
        condition->print(indent + 4);
        std::cout << std::string(indent + 2, ' ') << "Then:\n";
        thenBranch->print(indent + 4);
        if (elseBranch) {
            std::cout << std::string(indent + 2, ' ') << "Else:\n";
            elseBranch->print(indent + 4);
        }
    }
};

// while语句节点
class WhileStmt : public Stmt {
public:
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> body;

    WhileStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body)
        : condition(std::move(condition)), body(std::move(body)) {}
        
    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "While:\n";
        std::cout << std::string(indent + 2, ' ') << "Condition:\n";
        condition->print(indent + 4);
        std::cout << std::string(indent + 2, ' ') << "Body:\n";
        body->print(indent + 4);
    }
};

// do-while语句节点
class DoWhileStmt : public Stmt {
public:
    std::unique_ptr<Stmt> body;
    std::unique_ptr<Expr> condition;

    DoWhileStmt(std::unique_ptr<Stmt> body, std::unique_ptr<Expr> condition)
        : body(std::move(body)), condition(std::move(condition)) {}
        
    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "Do-While:\n";
        std::cout << std::string(indent + 2, ' ') << "Body:\n";
        body->print(indent + 4);
        std::cout << std::string(indent + 2, ' ') << "Condition:\n";
        condition->print(indent + 4);
    }
};

// break语句节点
class BreakStmt : public Stmt {
public:
    Token keyword;  // break关键字的token，用于错误报告

    explicit BreakStmt(Token keyword) : keyword(keyword) {}
    
    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "Break\n";
    }
};

// 赋值语句节点
class AssignStmt : public Stmt {
public:
    Token name;  // 标识符
    std::unique_ptr<Expr> value;

    AssignStmt(Token name, std::unique_ptr<Expr> value)
        : name(name), value(std::move(value)) {}
        
    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "Assign(" << name.lexeme << "):\n";
        value->print(indent + 2);
    }
};

// 解析器类
class Parser {
public:
    explicit Parser(const std::string& source);
    std::unique_ptr<BlockStmt> parse();

private:
    Lexer lexer;
    Token currentToken;
    bool hadError;
    bool panicMode;

    // 递归下降解析函数
    std::unique_ptr<BlockStmt> program();
    std::unique_ptr<BlockStmt> block();
    std::unique_ptr<Stmt> statement();
    std::unique_ptr<Expr> expression();
    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> factor();
    std::unique_ptr<Expr> boolExpr();

    // 辅助函数
    void advance();
    bool check(TokenType type) const;
    bool match(TokenType type);
    void consume(TokenType type, const std::string& message);
    void synchronize();
    void error(const std::string& message);
    ParseError errorAt(Token token, const std::string& message);
};

#endif // PARSER_H
