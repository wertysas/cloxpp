//
// Created by Johan Ericsson on 4/24/23.
//

#ifndef CLOXPP_PARSER_HPP
#define CLOXPP_PARSER_HPP


#include "common.hpp"
#include "token.hpp"
#include "error_handling.hpp"
#include "chunk.hpp"


// errorAtCurrent -> error_reporter_.report(parser.current, message)
// error -> error_report_.report(parser.previous, message)

class Parser {
    uint current_;
    Chunk& chunk_;
    const std::vector<Token>& tokens_;
    ErrorReporter& error_reporter_;
    bool panic_mode_=false;
public:
    Parser(const std::vector<Token>& tokens,Chunk& chunk, ErrorReporter& error_reporter) :
            current_(0),
            chunk_(chunk),
            tokens_(tokens), error_reporter_(error_reporter) {};
    void advance();
    void error(uint idx, const char *message);
    bool had_error() const { return error_reporter_.has_error; };
    void consume(TokenType type, const char *message);

    void emit_byte(OpCode opcode);
};


#endif //CLOXPP_PARSER_HPP
