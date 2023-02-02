//
// Created by Johan Ericsson on 2023-01-26.
//

#ifndef CLOXPP_TOKEN_HPP
#define CLOXPP_TOKEN_HPP

#include <string>

using namespace std;

enum TokenType {
    // Single char tokens
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    COMMA,
    DOT,
    MINUS,
    PLUS,
    SEMICOLON,
    SLASH,
    STAR,

    // Single OR Double char tokens
    BANG,
    BANG_EQUAL,
    EQUAL,
    EQUAL_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL,

    // Literals
    IDENTIFIER,
    STRING,
    NUMBER,

    // Keywords
    AND,
    CLASS,
    ELSE,
    FALSE,
    FUN,
    FOR,
    IF,
    NIL,
    OR,
    PRINT,
    RETURN,
    SUPER,
    THIS,
    TRUE,
    VAR,
    WHILE,

    // End of file is a special token
    END_OF_FILE
};

class Token {
    void* literal;
    int line;
    public:
    Token(TokenType type_, string lexeme_, void *literal_, int line_);

    string lexeme;
    TokenType type;
};


#endif //CLOXPP_TOKEN_HPP
