#ifndef LEXER_H
#define LEXER_H

#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

enum class TokenType {
    // Keywords
    IF,
    ELSE,
    WHILE,
    DO,
    MAIN,
    INT,
    FLOAT,
    DOUBLE,
    RETURN,
    CONST,
    VOID,
    CONTINUE,
    BREAK,
    CHAR,
    UNSIGNED,
    ENUM,
    LONG,
    SWITCH,
    CASE,
    AUTO,
    STATIC,
    TYPEDEF,
    STRUCT,
    UNION,
    FOR,
    DEFAULT,
    EXTERN,
    REGISTER,
    SIGNED,
    SIZEOF,
    GOTO,
    VOLATILE,
    SHORT,

    // Special symbols
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    MOD, // + - * / %
    INCREMENT,
    DECREMENT, // ++ --
    ASSIGN,    // =
    LESS,
    GREATER,
    LBRACE,
    RBRACE,    // < > { }
    SEMICOLON, // ;
    LPAREN,
    RPAREN, // ( )
    LBRACKET,
    RBRACKET, // [ ]
    EQUAL,
    NOT_EQUAL, // == !=
    AND,
    OR,
    NOT, // && || !
    GREATER_EQUAL,
    LESS_EQUAL, // >= <=
    ADD_ASSIGN,
    SUB_ASSIGN,
    MUL_ASSIGN,
    DIV_ASSIGN,
    MOD_ASSIGN, //+= -= *= /= %=
    AND_ASSIGN,
    OR_ASSIGN,
    XOR_ASSIGN, // &= |= ^=
    COLON,      // :
    QUESTION,   // ?
    COMMA,      // ,
    BITWISE_AND,
    BITWISE_OR,
    BITWISE_XOR,
    BITWISE_NOT, // & | ^ ~
    SHIFT_LEFT,
    SHIFT_RIGHT, // << >>
    ADDRESS_OF,  // & (when used for address)
    SQUOTE,
    DQUOTE, // ' "

    // Other tokens
    IDENTIFIER,      // identifiers
    INTEGER_LITERAL, // integers
    FLOAT_LITERAL,   // floating point numbers
    CHAR_LITERAL,    // character literals
    STRING_LITERAL,  // string literals
    END_OF_FILE,     // end of file
    ERROR            // error token
};

struct Token {
    TokenType type;
    std::string lexeme;
    int line;
    int column;

    // 添加默认构造函数
    Token() : type(TokenType::ERROR), lexeme(""), line(0), column(0) {}
    
    Token(TokenType t, const std::string &l, int ln, int col)
        : type(t), lexeme(l), line(ln), column(col) {}
};

class Lexer {
public:
    Lexer(const std::string &source);
    Token getToken();
    const std::vector<Token> &getAllTokens();
    void printTokens() const;
    static std::string getTokenName(TokenType type);

private:
    std::string source;
    size_t currentPos;
    int currentLine;
    int currentColumn;
    std::vector<Token> tokens;
    static std::unordered_map<std::string, TokenType> keywords;

    char peek() const;
    char advance();
    bool match(char expected);
    void skipWhitespace();
    void skipComment();
    Token handleIdentifier();
    Token handleNumber();
    Token handleString();
    Token handleChar();
    Token handleOperator();
    bool isAtEnd() const;
};

#endif // LEXER_H
