//
// Created by Johan Ericsson on 2023-01-26.
//

#include "token.hpp"

Token::Token(TokenType type_, string lexeme_, void *literal_, int line_) : type(type_), lexeme(lexeme_), literal(literal_),  line(line_) { }

