#include "lexer.h"
#include <iostream>
#include <cctype>
#include <stdexcept>

std::unordered_map<std::string, TokenType> Lexer::keywords = {
    {"if", TokenType::IF},
    {"else", TokenType::ELSE},
    {"while", TokenType::WHILE},
    {"do", TokenType::DO},
    {"main", TokenType::MAIN},
    {"int", TokenType::INT},
    {"float", TokenType::FLOAT},
    {"double", TokenType::DOUBLE},
    {"return", TokenType::RETURN},
    {"const", TokenType::CONST},
    {"void", TokenType::VOID},
    {"continue", TokenType::CONTINUE},
    {"break", TokenType::BREAK},
    {"char", TokenType::CHAR},
    {"unsigned", TokenType::UNSIGNED},
    {"enum", TokenType::ENUM},
    {"long", TokenType::LONG},
    {"switch", TokenType::SWITCH},
    {"case", TokenType::CASE},
    {"auto", TokenType::AUTO},
    {"static", TokenType::STATIC},
    {"for", TokenType::FOR},
    {"default", TokenType::DEFAULT},
    {"typedef", TokenType::TYPEDEF},
    {"struct", TokenType::STRUCT},
    {"union", TokenType::UNION},
    {"sizeof", TokenType::SIZEOF},
    {"goto", TokenType::GOTO},
    {"extern", TokenType::EXTERN},
    {"register", TokenType::REGISTER},
    {"volatile", TokenType::VOLATILE},
    {"signed", TokenType::SIGNED},
    {"short", TokenType::SHORT}
};

Lexer::Lexer(const std::string& source) 
    : source(source), currentPos(0), currentLine(1), currentColumn(1) {}

char Lexer::peek() const {
    if (isAtEnd()) return '\0';
    return source[currentPos];
}

char Lexer::advance() {
    char current = peek();
    currentPos++;
    if (current == '\n') {
        currentLine++;
        currentColumn = 1;
    } else {
        currentColumn++;
    }
    return current;
}

bool Lexer::match(char expected) {
    if (isAtEnd() || source[currentPos] != expected) return false;
    advance();
    return true;
}

void Lexer::skipWhitespace() {
    while (!isAtEnd()) {
        char c = peek();
        switch (c) {
            case ' ':
            case '\t':
            case '\r':
                advance();
                break;
            case '\n':
                advance();
                break;
            case '{':
                if (source[currentPos + 1] == '*') {
                    skipComment();
                } else {
                    return;
                }
                break;
            case '/':
                if (source[currentPos + 1] == '/') {
                    while (peek() != '\n' && !isAtEnd()) advance();
                } else {
                    return;
                }
                break;
            default:
                return;
        }
    }
}

void Lexer::skipComment() {
    int startLine = currentLine;
    int startColumn = currentColumn;
    
    advance(); // Skip {
    advance(); // Skip *
    
    while (!isAtEnd()) {
        if (peek() == '*' && source[currentPos + 1] == '}') {
            advance(); // Skip *
            advance(); // Skip }
            return;
        }
        advance();
    }
    
    // 如果注释未关闭
    throw std::runtime_error(
        "Unterminated comment started at line " + std::to_string(startLine) + 
        ", column " + std::to_string(startColumn)
    );
}

Token Lexer::handleIdentifier() {
    std::string identifier;
    int startLine = currentLine;
    int startColumn = currentColumn;
    
    while (!isAtEnd() && (isalnum(peek()) || peek() == '_')) {
        identifier += advance();
    }
    
    // Check for keywords
    if (identifier == "int") return Token(TokenType::INT, identifier, startLine, startColumn);
    if (identifier == "float") return Token(TokenType::FLOAT, identifier, startLine, startColumn);
    if (identifier == "char") return Token(TokenType::CHAR, identifier, startLine, startColumn);
    if (identifier == "void") return Token(TokenType::VOID, identifier, startLine, startColumn);
    if (identifier == "if") return Token(TokenType::IF, identifier, startLine, startColumn);
    if (identifier == "else") return Token(TokenType::ELSE, identifier, startLine, startColumn);
    if (identifier == "while") return Token(TokenType::WHILE, identifier, startLine, startColumn);
    if (identifier == "for") return Token(TokenType::FOR, identifier, startLine, startColumn);
    if (identifier == "return") return Token(TokenType::RETURN, identifier, startLine, startColumn);
    if (identifier == "break") return Token(TokenType::BREAK, identifier, startLine, startColumn);
    if (identifier == "continue") return Token(TokenType::CONTINUE, identifier, startLine, startColumn);
    if (identifier == "do") return Token(TokenType::DO, identifier, startLine, startColumn);
    if (identifier == "switch") return Token(TokenType::SWITCH, identifier, startLine, startColumn);
    if (identifier == "case") return Token(TokenType::CASE, identifier, startLine, startColumn);
    if (identifier == "default") return Token(TokenType::DEFAULT, identifier, startLine, startColumn);
    if (identifier == "typedef") return Token(TokenType::TYPEDEF, identifier, startLine, startColumn);
    if (identifier == "struct") return Token(TokenType::STRUCT, identifier, startLine, startColumn);
    if (identifier == "union") return Token(TokenType::UNION, identifier, startLine, startColumn);
    if (identifier == "enum") return Token(TokenType::ENUM, identifier, startLine, startColumn);
    if (identifier == "sizeof") return Token(TokenType::SIZEOF, identifier, startLine, startColumn);
    if (identifier == "goto") return Token(TokenType::GOTO, identifier, startLine, startColumn);
    if (identifier == "auto") return Token(TokenType::AUTO, identifier, startLine, startColumn);
    if (identifier == "static") return Token(TokenType::STATIC, identifier, startLine, startColumn);
    if (identifier == "extern") return Token(TokenType::EXTERN, identifier, startLine, startColumn);
    if (identifier == "register") return Token(TokenType::REGISTER, identifier, startLine, startColumn);
    if (identifier == "const") return Token(TokenType::CONST, identifier, startLine, startColumn);
    if (identifier == "volatile") return Token(TokenType::VOLATILE, identifier, startLine, startColumn);
    if (identifier == "signed") return Token(TokenType::SIGNED, identifier, startLine, startColumn);
    if (identifier == "unsigned") return Token(TokenType::UNSIGNED, identifier, startLine, startColumn);
    if (identifier == "long") return Token(TokenType::LONG, identifier, startLine, startColumn);
    if (identifier == "short") return Token(TokenType::SHORT, identifier, startLine, startColumn);
    if (identifier == "double") return Token(TokenType::DOUBLE, identifier, startLine, startColumn);
    
    return Token(TokenType::IDENTIFIER, identifier, startLine, startColumn);
}

Token Lexer::handleNumber() {
    std::string number;
    int startLine = currentLine;
    int startColumn = currentColumn;
    bool isFloat = false;
    int dotCount = 0;
    
    // Check for hex, octal, or binary prefix
    if (peek() == '0') {
        number += advance();
        if (!isAtEnd()) {
            char next = peek();
            if (next == 'x' || next == 'X') {  // Hexadecimal
                number += advance();
                while (!isAtEnd() && (isdigit(peek()) || 
                       (peek() >= 'a' && peek() <= 'f') || 
                       (peek() >= 'A' && peek() <= 'F'))) {
                    number += advance();
                }
                if (number.length() <= 2) {  // Only "0x"
                    throw std::runtime_error(
                        "Invalid hexadecimal number at line " + std::to_string(startLine) + 
                        ", column " + std::to_string(startColumn)
                    );
                }
                return Token(TokenType::INTEGER_LITERAL, number, startLine, startColumn);
            }
            else if (next == 'b' || next == 'B') {  // Binary
                number += advance();
                while (!isAtEnd() && (peek() == '0' || peek() == '1')) {
                    number += advance();
                }
                if (number.length() <= 2) {  // Only "0b"
                    throw std::runtime_error(
                        "Invalid binary number at line " + std::to_string(startLine) + 
                        ", column " + std::to_string(startColumn)
                    );
                }
                return Token(TokenType::INTEGER_LITERAL, number, startLine, startColumn);
            }
            else if (isdigit(next) && next < '8') {  // Octal
                while (!isAtEnd() && isdigit(peek()) && peek() < '8') {
                    number += advance();
                }
                return Token(TokenType::INTEGER_LITERAL, number, startLine, startColumn);
            }
        }
    }
    
    // Decimal number
    while (!isAtEnd() && (isdigit(peek()) || peek() == '.')) {
        if (peek() == '.') {
            dotCount++;
            if (dotCount > 1) {
                throw std::runtime_error(
                    "Invalid number format at line " + std::to_string(startLine) + 
                    ", column " + std::to_string(startColumn)
                );
            }
            isFloat = true;
        }
        number += advance();
    }
    
    // Check for suffixes (L, U, F, etc.)
    if (!isAtEnd()) {
        char suffix = peek();
        if (suffix == 'L' || suffix == 'l' || 
            suffix == 'U' || suffix == 'u' || 
            suffix == 'F' || suffix == 'f') {
            number += advance();
        }
    }
    
    // Check for invalid float format
    if (isFloat && (number.back() == '.' || number.front() == '.')) {
        throw std::runtime_error(
            "Invalid floating point number at line " + std::to_string(startLine) + 
            ", column " + std::to_string(startColumn)
        );
    }
    
    return Token(isFloat ? TokenType::FLOAT_LITERAL : TokenType::INTEGER_LITERAL,
                number, startLine, startColumn);
}

Token Lexer::handleString() {
    std::string str;
    int startLine = currentLine;
    int startColumn = currentColumn;
    advance(); // Skip opening quote
    
    while (!isAtEnd() && peek() != '"') {
        if (peek() == '\\') {
            advance();
            if (isAtEnd()) {
                throw std::runtime_error(
                    "Unterminated string at line " + std::to_string(startLine) + 
                    ", column " + std::to_string(startColumn)
                );
            }
            switch (peek()) {
                case 'n': str += '\n'; break;
                case 't': str += '\t'; break;
                case 'r': str += '\r'; break;
                case '\\': str += '\\'; break;
                case '"': str += '"'; break;
                default: 
                    throw std::runtime_error(
                        "Invalid escape sequence in string at line " + std::to_string(currentLine) + 
                        ", column " + std::to_string(currentColumn)
                    );
            }
            advance();
        } else {
            str += advance();
        }
    }
    
    if (isAtEnd()) {
        throw std::runtime_error(
            "Unterminated string at line " + std::to_string(startLine) + 
            ", column " + std::to_string(startColumn)
        );
    }
    
    advance(); // Skip closing quote
    return Token(TokenType::STRING_LITERAL, str, startLine, startColumn);
}

Token Lexer::handleChar() {
    int startLine = currentLine;
    int startColumn = currentColumn;
    advance(); // Skip opening quote
    
    std::string ch;
    if (isAtEnd() || peek() == '\'') {
        throw std::runtime_error(
            "Empty character literal at line " + std::to_string(startLine) + 
            ", column " + std::to_string(startColumn)
        );
    }
    
    if (peek() == '\\') {
        ch = "\\";
        advance(); // consume backslash
        if (isAtEnd()) {
            throw std::runtime_error(
                "Unterminated escape sequence at line " + std::to_string(startLine) + 
                ", column " + std::to_string(startColumn)
            );
        }
        ch += peek();
        switch (peek()) {
            case 'n': 
            case 't': 
            case 'r': 
            case '\\': 
            case '\'': 
            case '\"': 
            case '0': 
                advance(); // consume escape character
                break;
            default: 
                throw std::runtime_error(
                    "Invalid escape sequence '\\" + std::string(1, peek()) + 
                    "' at line " + std::to_string(currentLine) + 
                    ", column " + std::to_string(currentColumn)
                );
        }
    } else {
        ch = std::string(1, advance()); // consume the character
    }
    
    if (isAtEnd() || peek() != '\'') {
        throw std::runtime_error(
            "Unterminated character literal at line " + std::to_string(startLine) + 
            ", column " + std::to_string(startColumn)
        );
    }
    
    advance(); // consume closing quote
    return Token(TokenType::CHAR_LITERAL, ch, startLine, startColumn);
}

Token Lexer::handleOperator() {
    char current = peek();
    int startLine = currentLine;
    int startColumn = currentColumn;
    
    switch (advance()) {
        case '=':
            if (peek() == '=') {
                advance();
                return Token(TokenType::EQUAL, "==", startLine, startColumn);
            }
            return Token(TokenType::ASSIGN, "=", startLine, startColumn);
            
        case '+':
            if (peek() == '+') {
                advance();
                return Token(TokenType::INCREMENT, "++", startLine, startColumn);
            } else if (peek() == '=') {
                advance();
                return Token(TokenType::ADD_ASSIGN, "+=", startLine, startColumn);
            }
            return Token(TokenType::PLUS, "+", startLine, startColumn);
            
        case '-':
            if (peek() == '-') {
                advance();
                return Token(TokenType::DECREMENT, "--", startLine, startColumn);
            } else if (peek() == '=') {
                advance();
                return Token(TokenType::SUB_ASSIGN, "-=", startLine, startColumn);
            }
            return Token(TokenType::MINUS, "-", startLine, startColumn);
            
        case '*':
            if (peek() == '=') {
                advance();
                return Token(TokenType::MUL_ASSIGN, "*=", startLine, startColumn);
            }
            return Token(TokenType::MULTIPLY, "*", startLine, startColumn);
            
        case '/':
            if (peek() == '=') {
                advance();
                return Token(TokenType::DIV_ASSIGN, "/=", startLine, startColumn);
            }
            return Token(TokenType::DIVIDE, "/", startLine, startColumn);
            
        case '%':
            if (peek() == '=') {
                advance();
                return Token(TokenType::MOD_ASSIGN, "%=", startLine, startColumn);
            }
            return Token(TokenType::MOD, "%", startLine, startColumn);
            
        case '&':
            if (peek() == '&') {
                advance();
                return Token(TokenType::AND, "&&", startLine, startColumn);
            } else if (peek() == '=') {
                advance();
                return Token(TokenType::AND_ASSIGN, "&=", startLine, startColumn);
            }
            return Token(TokenType::BITWISE_AND, "&", startLine, startColumn);
            
        case '|':
            if (peek() == '|') {
                advance();
                return Token(TokenType::OR, "||", startLine, startColumn);
            } else if (peek() == '=') {
                advance();
                return Token(TokenType::OR_ASSIGN, "|=", startLine, startColumn);
            }
            return Token(TokenType::BITWISE_OR, "|", startLine, startColumn);
            
        case '^':
            return Token(TokenType::BITWISE_XOR, "^", startLine, startColumn);
            
        case '~':
            return Token(TokenType::BITWISE_NOT, "~", startLine, startColumn);
            
        case '{': return Token(TokenType::LBRACE, "{", startLine, startColumn);
        case '}': return Token(TokenType::RBRACE, "}", startLine, startColumn);
        case '(': return Token(TokenType::LPAREN, "(", startLine, startColumn);
        case ')': return Token(TokenType::RPAREN, ")", startLine, startColumn);
        case '[': return Token(TokenType::LBRACKET, "[", startLine, startColumn);
        case ']': return Token(TokenType::RBRACKET, "]", startLine, startColumn);
        case ';': return Token(TokenType::SEMICOLON, ";", startLine, startColumn);
        case '\'': return handleChar();
        case '"': return handleString();
        case ':': 
            return Token(TokenType::COLON, ":", startLine, startColumn);
        case '?':
            return Token(TokenType::QUESTION, "?", startLine, startColumn);
        case ',':
            return Token(TokenType::COMMA, ",", startLine, startColumn);
        case '<':
            if (peek() == '=') {
                advance();
                return Token(TokenType::LESS_EQUAL, "<=", startLine, startColumn);
            } else if (peek() == '<') {
                advance();
                return Token(TokenType::SHIFT_LEFT, "<<", startLine, startColumn);
            }
            return Token(TokenType::LESS, "<", startLine, startColumn);

        case '>':
            if (peek() == '=') {
                advance();
                return Token(TokenType::GREATER_EQUAL, ">=", startLine, startColumn);
            } else if (peek() == '>') {
                advance();
                return Token(TokenType::SHIFT_RIGHT, ">>", startLine, startColumn);
            }
            return Token(TokenType::GREATER, ">", startLine, startColumn);

        case '!':
            if (peek() == '=') {
                advance();
                return Token(TokenType::NOT_EQUAL, "!=", startLine, startColumn);
            }
            return Token(TokenType::NOT, "!", startLine, startColumn);
        default: 
            throw std::runtime_error(
                "Unknown operator '" + std::string(1, current) + "' at line " + 
                std::to_string(startLine) + ", column " + std::to_string(startColumn)
            );
    }

    throw std::runtime_error("Unreachable code in handleOperator");
}

Token Lexer::getToken() {
    skipWhitespace();
    
    if (isAtEnd()) {
        return Token(TokenType::END_OF_FILE, "", currentLine, currentColumn);
    }
    
    char c = peek();
    
    // 检查数字开头
    if (isdigit(c)) {
        // 先看看后面是否紧跟着字母或下划线
        size_t pos = currentPos;
        while (pos < source.length() && (isalnum(source[pos]) || source[pos] == '_')) {
            pos++;
        }
        
        // 如果数字后面紧跟着字母或下划线，且不是合法的数字字面量前缀（0x, 0b等），则是非法标识符
        if (pos > currentPos + 1) {
            char nextChar = source[currentPos + 1];
            bool isValidPrefix = (c == '0' && (nextChar == 'x' || nextChar == 'X' || 
                                              nextChar == 'b' || nextChar == 'B' ||
                                              (nextChar >= '0' && nextChar <= '7')));
            if (!isValidPrefix && (isalpha(nextChar) || nextChar == '_')) {
                std::string invalid;
                int startColumn = currentColumn;
                while (!isAtEnd() && (isalnum(peek()) || peek() == '_')) {
                    invalid += advance();
                }
                throw std::runtime_error(
                    "Invalid identifier '" + invalid + "' starting with number at line " + 
                    std::to_string(currentLine) + ", column " + std::to_string(startColumn)
                );
            }
        }
        return handleNumber();
    }
    
    if (isalpha(c) || c == '_') {
        return handleIdentifier();
    }
    
    if (c == '\'') {
        return handleChar();
    }
    
    return handleOperator();
}

const std::vector<Token>& Lexer::getAllTokens() {
    while (true) {
        Token token = getToken();
        tokens.push_back(token);
        if (token.type == TokenType::END_OF_FILE) break;
    }
    return tokens;
}

void Lexer::printTokens() const {
    for (const auto& token : tokens) {
        std::cout << "Line " << token.line << ", Column " << token.column 
                  << ": [" << getTokenName(token.type) << "] " 
                  << token.lexeme << std::endl;
    }
}

std::string Lexer::getTokenName(TokenType type) {
    switch (type) {
        case TokenType::IF: return "IF";
        case TokenType::ELSE: return "ELSE";
        case TokenType::WHILE: return "WHILE";
        case TokenType::DO: return "DO";
        case TokenType::MAIN: return "MAIN";
        case TokenType::INT: return "INT";
        case TokenType::FLOAT: return "FLOAT";
        case TokenType::DOUBLE: return "DOUBLE";
        case TokenType::RETURN: return "RETURN";
        case TokenType::CONST: return "CONST";
        case TokenType::VOID: return "VOID";
        case TokenType::CONTINUE: return "CONTINUE";
        case TokenType::BREAK: return "BREAK";
        case TokenType::CHAR: return "CHAR";
        case TokenType::UNSIGNED: return "UNSIGNED";
        case TokenType::ENUM: return "ENUM";
        case TokenType::LONG: return "LONG";
        case TokenType::SWITCH: return "SWITCH";
        case TokenType::CASE: return "CASE";
        case TokenType::AUTO: return "AUTO";
        case TokenType::STATIC: return "STATIC";
        case TokenType::PLUS: return "PLUS";
        case TokenType::MINUS: return "MINUS";
        case TokenType::INCREMENT: return "INCREMENT";
        case TokenType::DECREMENT: return "DECREMENT";
        case TokenType::MULTIPLY: return "MULTIPLY";
        case TokenType::DIVIDE: return "DIVIDE";
        case TokenType::MOD: return "MOD";
        case TokenType::ASSIGN: return "ASSIGN";
        case TokenType::LESS: return "LESS";
        case TokenType::GREATER: return "GREATER";
        case TokenType::LBRACE: return "LBRACE";
        case TokenType::RBRACE: return "RBRACE";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::LPAREN: return "LPAREN";
        case TokenType::RPAREN: return "RPAREN";
        case TokenType::LBRACKET: return "LBRACKET";
        case TokenType::RBRACKET: return "RBRACKET";
        case TokenType::SQUOTE: return "SQUOTE";
        case TokenType::DQUOTE: return "DQUOTE";
        case TokenType::EQUAL: return "EQUAL";
        case TokenType::NOT_EQUAL: return "NOT_EQUAL";
        case TokenType::AND: return "AND";
        case TokenType::OR: return "OR";
        case TokenType::GREATER_EQUAL: return "GREATER_EQUAL";
        case TokenType::LESS_EQUAL: return "LESS_EQUAL";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::INTEGER_LITERAL: return "INTEGER_LITERAL";
        case TokenType::FLOAT_LITERAL: return "FLOAT_LITERAL";
        case TokenType::CHAR_LITERAL: return "CHAR_LITERAL";
        case TokenType::STRING_LITERAL: return "STRING_LITERAL";
        case TokenType::END_OF_FILE: return "EOF";
        case TokenType::ERROR: return "ERROR";
        case TokenType::ADD_ASSIGN: return "ADD_ASSIGN";
        case TokenType::SUB_ASSIGN: return "SUB_ASSIGN";
        case TokenType::MUL_ASSIGN: return "MUL_ASSIGN";
        case TokenType::DIV_ASSIGN: return "DIV_ASSIGN";
        case TokenType::MOD_ASSIGN: return "MOD_ASSIGN";
        case TokenType::COLON: return "COLON";
        case TokenType::QUESTION: return "QUESTION";
        case TokenType::COMMA: return "COMMA";
        case TokenType::NOT: return "NOT";
        case TokenType::BITWISE_AND: return "BITWISE_AND";
        case TokenType::BITWISE_OR: return "BITWISE_OR";
        case TokenType::BITWISE_XOR: return "BITWISE_XOR";
        case TokenType::BITWISE_NOT: return "BITWISE_NOT";
        case TokenType::SHIFT_LEFT: return "SHIFT_LEFT";
        case TokenType::SHIFT_RIGHT: return "SHIFT_RIGHT";
        case TokenType::ADDRESS_OF: return "ADDRESS_OF";
        case TokenType::FOR: return "FOR";
        case TokenType::DEFAULT: return "DEFAULT";
        case TokenType::TYPEDEF: return "TYPEDEF";
        case TokenType::STRUCT: return "STRUCT";
        case TokenType::UNION: return "UNION";
        case TokenType::SIZEOF: return "SIZEOF";
        case TokenType::GOTO: return "GOTO";
        case TokenType::EXTERN: return "EXTERN";
        case TokenType::REGISTER: return "REGISTER";
        case TokenType::VOLATILE: return "VOLATILE";
        case TokenType::SIGNED: return "SIGNED";
        case TokenType::SHORT: return "SHORT";
        default: return "UNKNOWN";
    }
}

bool Lexer::isAtEnd() const {
    return currentPos >= source.length();
}
