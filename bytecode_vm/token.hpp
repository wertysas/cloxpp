//
// Created by Johan Ericsson on 2023-04-21.
//

#ifndef CLOXPP_TOKEN_HPP
#define CLOXPP_TOKEN_HPP

#include "common.hpp"

enum TokenType : uint8_t {
    // Single-character tokens.
    TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN,
    TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,
    TOKEN_COMMA, TOKEN_DOT, TOKEN_MINUS, TOKEN_PLUS,
    TOKEN_SEMICOLON, TOKEN_SLASH, TOKEN_STAR,
    // One or two character tokens.
    TOKEN_BANG, TOKEN_BANG_EQUAL,
    TOKEN_EQUAL, TOKEN_EQUAL_EQUAL,
    TOKEN_GREATER, TOKEN_GREATER_EQUAL,
    TOKEN_LESS, TOKEN_LESS_EQUAL,
    // Literals.
    TOKEN_IDENTIFIER, TOKEN_STRING, TOKEN_NUMBER,
    // Keywords.
    TOKEN_AND, TOKEN_CLASS, TOKEN_ELSE, TOKEN_FALSE,
    TOKEN_FOR, TOKEN_FUN, TOKEN_IF, TOKEN_NIL, TOKEN_OR,
    TOKEN_PRINT, TOKEN_RETURN, TOKEN_SUPER, TOKEN_THIS,
    TOKEN_TRUE, TOKEN_VAR, TOKEN_WHILE,

    TOKEN_ERROR, TOKEN_EOF
};

struct Token {
    TokenType type;
    const char* start;
    uint length;
    uint line;
    Token(TokenType type_, const char* start_, uint length_, uint line_):
        type(type_), start(start_), length(length_), line(line_)  {};

};

// void error(const Token& token, const char* message) {
//     std::cerr << "[line " << token.line << "] Error";
//     if (token.type_ == TOKEN_EOF) {
//         std::cerr << " at end of file";
//     } else if (token.type_ == TOKEN_ERROR) {
//         // NOTHING
//     }
// }

static const char* token_types[] = {
    // Single-character tokens.
    "TOKEN_LEFT_PAREN",
    "TOKEN_RIGHT_PAREN",
    "TOKEN_LEFT_BRACE",
    "TOKEN_RIGHT_BRACE",
    "TOKEN_COMMA",
    "TOKEN_DOT",
    "TOKEN_MINUS",
    "TOKEN_PLUS",
    "TOKEN_SEMICOLON",
    "TOKEN_SLASH",
    "TOKEN_STAR",
    // One or two character tokens.
    "TOKEN_BANG",
    "TOKEN_BANG_EQUAL",
    "TOKEN_EQUAL",
    "TOKEN_EQUAL_EQUAL",
    "TOKEN_GREATER",
    "TOKEN_GREATER_EQUAL",
    "TOKEN_LESS",
    "TOKEN_LESS_EQUAL",
    // Literals.
    "TOKEN_IDENTIFIER",
    "TOKEN_STRING",
    "TOKEN_NUMBER",
    // Keywords.
    "TOKEN_AND",
    "TOKEN_CLASS",
    "TOKEN_ELSE",
    "TOKEN_FALSE",
    "TOKEN_FOR",
    "TOKEN_FUN",
    "TOKEN_IF",
    "TOKEN_NIL",
    "TOKEN_OR",
    "TOKEN_PRINT",
    "TOKEN_RETURN",
    "TOKEN_SUPER",
    "TOKEN_THIS",
    "TOKEN_TRUE",
    "TOKEN_VAR",
    "TOKEN_WHILE",

    "TOKEN_ERROR",
    "TOKEN_EOF"

};

const char* token_to_string(TokenType type);

#endif //CLOXPP_TOKEN_HPP
