//
// Created by Johan Ericsson on 4/24/23.
//

#ifndef CLOXPP_PARSER_HPP
#define CLOXPP_PARSER_HPP


#include "common.hpp"
#include "token.hpp"
#include "error_handling.hpp"



class Parser {
    uint current_;
    const std::vector<Token>& tokens_;
    const ErrorReporter& error_reporter_;
    void error(const char* message);
public:
    Parser(const std::vector<Token>& tokens, const ErrorReporter& error_reporter) :
            current_(0),
            tokens_(tokens),
            error_reporter_(error_reporter) {};
    void advance();
};


#endif //CLOXPP_PARSER_HPP
