//
// Created by Johan Ericsson on 4/24/23.
//

#ifndef CLOXPP_PARSER_HPP
#define CLOXPP_PARSER_HPP


#include "token.hpp"

class Parser {
    uint current_;
    const std::vector<Token>& tokens_;
    void error(const char* message);
public:
    Parser(const std::vector<Token>& tokens) : current_(0), tokens_(tokens) {};
    void advance()
};


#endif //CLOXPP_PARSER_HPP
