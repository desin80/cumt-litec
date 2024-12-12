#include "parser.h"

Parser::Parser(const std::string& source) 
    : lexer(source), hadError(false), panicMode(false) {
    currentToken = lexer.getToken(); // 初始化时获取第一个token
}

void Parser::advance() {
    currentToken = lexer.getToken();
}

bool Parser::check(TokenType type) const {
    return currentToken.type == type;
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

void Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) {
        advance();
        return;
    }
    throw errorAt(currentToken, message);
}

ParseError Parser::errorAt(Token token, const std::string& message) {
    if (panicMode) return ParseError(message);
    panicMode = true;
    hadError = true;

    std::string errorMsg = "Error at line " + std::to_string(token.line) +
                          ", column " + std::to_string(token.column) + ": " +
                          message;
    if (token.type == TokenType::END_OF_FILE) {
        errorMsg += " at end of file";
    } else {
        errorMsg += " at '" + token.lexeme + "'";
    }

    return ParseError(errorMsg);
}

void Parser::error(const std::string& message) {
    throw errorAt(currentToken, message);
}

void Parser::synchronize() {
    advance();
    
    while (!check(TokenType::END_OF_FILE)) {
        if (currentToken.type == TokenType::SEMICOLON) {
            advance();
            return;
        }
        
        switch (currentToken.type) {
            case TokenType::IF:
            case TokenType::WHILE:
            case TokenType::DO:
            case TokenType::BREAK:
            case TokenType::LBRACE:
                return;
            default:
                advance();
        }
    }
}

std::unique_ptr<BlockStmt> Parser::parse() {
    try {
        return program();
    } catch (const ParseError& error) {
        return nullptr;
    }
}

std::unique_ptr<BlockStmt> Parser::program() {
    return block();
}

std::unique_ptr<BlockStmt> Parser::block() {
    std::vector<std::unique_ptr<Stmt>> statements;
    
    try {
        while (!check(TokenType::RBRACE) && !check(TokenType::END_OF_FILE)) {
            auto stmt = statement();
            if (stmt) {
                statements.push_back(std::move(stmt));
            }
        }
        
        consume(TokenType::RBRACE, "Expect '}' after block");
    } catch (const ParseError& e) {
        synchronize();
    }
    
    return std::make_unique<BlockStmt>(std::move(statements));
}

std::unique_ptr<Stmt> Parser::statement() {
    try {
        if (match(TokenType::IF)) {
            consume(TokenType::LPAREN, "Expect '(' after 'if'");
            auto condition = boolExpr();
            consume(TokenType::RPAREN, "Expect ')' after condition");
            
            auto thenBranch = statement();
            std::unique_ptr<Stmt> elseBranch = nullptr;
            
            if (match(TokenType::ELSE)) {
                elseBranch = statement();
            }
            
            return std::make_unique<IfStmt>(
                std::move(condition),
                std::move(thenBranch),
                std::move(elseBranch)
            );
        }
        
        if (match(TokenType::WHILE)) {
            consume(TokenType::LPAREN, "Expect '(' after 'while'");
            auto condition = boolExpr();
            consume(TokenType::RPAREN, "Expect ')' after condition");
            auto body = statement();
            
            return std::make_unique<WhileStmt>(
                std::move(condition),
                std::move(body)
            );
        }
        
        if (match(TokenType::DO)) {
            auto body = statement();
            consume(TokenType::WHILE, "Expect 'while' after do statement");
            consume(TokenType::LPAREN, "Expect '(' after 'while'");
            auto condition = boolExpr();
            consume(TokenType::RPAREN, "Expect ')' after condition");
            consume(TokenType::SEMICOLON, "Expect ';' after do-while statement");
            
            return std::make_unique<DoWhileStmt>(
                std::move(body),
                std::move(condition)
            );
        }
        
        if (match(TokenType::BREAK)) {
            Token breakToken = currentToken;
            consume(TokenType::SEMICOLON, "Expect ';' after 'break'");
            return std::make_unique<BreakStmt>(breakToken);
        }
        
        if (match(TokenType::LBRACE)) {
            return block();
        }
        
        // 赋值语句或表达式语句
        if (check(TokenType::IDENTIFIER)) {
            Token name = currentToken;
            advance();
            
            if (match(TokenType::ASSIGN)) {
                auto value = expression();
                consume(TokenType::SEMICOLON, "Expect ';' after expression");
                return std::make_unique<AssignStmt>(name, std::move(value));
            }
            
            // 回退，因为这可能是一个表达式
            currentToken = name;
        }
        
        // 表达式语句
        auto expr = expression();
        consume(TokenType::SEMICOLON, "Expect ';' after expression");
        return std::make_unique<ExpressionStmt>(std::move(expr));
    } catch (const ParseError& e) {
        synchronize();
        return nullptr;
    }
}

std::unique_ptr<Expr> Parser::expression() {
    auto left = term();
    
    while (check(TokenType::PLUS) || check(TokenType::MINUS)) {
        Token op = currentToken;
        advance();
        auto right = term();
        left = std::make_unique<BinaryExpr>(
            std::move(left),
            op.type,
            std::move(right)
        );
    }
    
    return left;
}

std::unique_ptr<Expr> Parser::term() {
    auto left = factor();
    
    while (check(TokenType::MULTIPLY) || check(TokenType::DIVIDE)) {
        Token op = currentToken;
        advance();
        auto right = factor();
        left = std::make_unique<BinaryExpr>(
            std::move(left),
            op.type,
            std::move(right)
        );
    }
    
    return left;
}

std::unique_ptr<Expr> Parser::factor() {
    if (match(TokenType::LPAREN)) {
        auto expr = expression();
        consume(TokenType::RPAREN, "Expect ')' after expression");
        return expr;
    }
    
    if (check(TokenType::IDENTIFIER)) {
        Token id = currentToken;
        advance();
        return std::make_unique<IdentifierExpr>(id);
    }
    
    if (check(TokenType::INTEGER_LITERAL) || check(TokenType::FLOAT_LITERAL)) {
        Token literal = currentToken;
        advance();
        return std::make_unique<LiteralExpr>(literal);
    }
    
    error("Expect expression");
    return nullptr;
}

std::unique_ptr<Expr> Parser::boolExpr() {
    auto left = expression();
    
    while (check(TokenType::LESS) || check(TokenType::LESS_EQUAL) ||
           check(TokenType::GREATER) || check(TokenType::GREATER_EQUAL) ||
           check(TokenType::EQUAL) || check(TokenType::NOT_EQUAL)) {
        Token op = currentToken;
        advance();
        auto right = expression();
        left = std::make_unique<BinaryExpr>(
            std::move(left),
            op.type,
            std::move(right)
        );
    }
    
    return left;
}
