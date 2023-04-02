//
// Created by Johan Ericsson on 2023-01-26.
//

#include "token.hpp"

const map<TokenType, std::string> token_table = {
    {LEFT_PAREN, "LEFT_PAREN"},
    {RIGHT_PAREN, "RIGHT_PAREN"},
    {LEFT_BRACE, "LEFT_BRACE"},
    {RIGHT_BRACE, "RIGHT_BRACE"},
    {COMMA, "COMMA"},
    {DOT, "DOT"      },
    {MINUS, "MINUS"    },
    {PLUS,      "PLUS"},
    {SEMICOLON, "SEMICOLON"},
    {SLASH,     "SLASH"},
    {STAR,      "STAR"},
    {BANG,      "BANG"},
    {BANG_EQUAL, "BANG_EQUAL"},
    {EQUAL,     "EQUAL"},
    {EQUAL_EQUAL,"EQUAL_EQUAL"},
    {GREATER,   "GREATER"},
    {GREATER_EQUAL, "GREATER_EQUAL"},
    {LESS,      "LESS"},
    {LESS_EQUAL, "LESS_EQUAL"},
    {IDENTIFIER, "IDENTIFIER"},
    {STRING,    "STRING"},
    {NUMBER,    "NUMBER"},
    {AND,       "AND"},
    {CLASS,     "CLASS"},
    {ELSE,      "ELSE"},
    {FALSE,     "FALSE"},
    {FUN,       "FUN"},
    {FOR,       "FOR"},
    {IF,        "IF"},
    {NIL,       "NIL"},
    {OR,        "OR"},
    {PRINT,     "PRINT"},
    {RETURN,    "RETURN"},
    {SUPER,     "SUPER"},
    {THIS,      "THIS"},
    {TRUE,      "TRUE"},
    {VAR,       "VAR"},
    {WHILE,     "WHILE"},
    {END_OF_FILE, "END_OF_FILE"}
};

Token::Token(TokenType type_, string lexeme_, void *literal_, int line_) : type(type_), lexeme(lexeme_), literal(literal_),  line(line_) { }
