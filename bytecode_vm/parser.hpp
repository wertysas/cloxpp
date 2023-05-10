//
// Created by Johan Ericsson on 4/24/23.
//

#ifndef CLOXPP_PARSER_HPP
#define CLOXPP_PARSER_HPP


#include "common.hpp"
#include "token.hpp"
#include "error_handling.hpp"
#include "chunk.hpp"



// Precedence enum where the different presedences are in order of importance
// thus PREC_X < PREC_Y if PREC_Y has presedence over PREC_X
enum Precedence: uint8_t {
    PREC_NONE,
    PREC_ASSIGNMENT,  // =
    PREC_OR,          // or
    PREC_AND,         // and
    PREC_EQUALITY,    // == !=
    PREC_COMPARISON,  // < > <= >=
    PREC_TERM,        // + -
    PREC_FACTOR,      // * /
    PREC_UNARY,       // ! -
    PREC_CALL,        // . ()
    PREC_PRIMARY
};

class Parser;
typedef void (Parser::* ParseFn)();

struct ParseRule {
    ParseFn prefix;
    ParseFn infix;
    Precedence precedence;
};

class Parser {
public:
    Parser(const std::vector<Token>& tokens,Chunk& chunk, ErrorReporter& error_reporter);
    void advance();
    void error(uint idx, const char *message);
    bool had_error() const { return error_reporter_.has_error; };
    void consume(TokenType type, const char *message);

    inline const Token& previous() const {return tokens_[previous_];}
    inline const Token& current() const {return tokens_[current_];}
    inline const Token& next_token();
    void number();
    void string();
    void grouping();
    void expression();
    void literal();
    void unary();
    void binary();
    void parse_precedence(Precedence precedence);
    ParseRule* parse_rule(TokenType type);

    void emit_byte(OpCode opcode);
    void emit_byte(uint token_idx, OpCode opcode);
    void emit_double_byte(OpCode op1, OpCode op2);

    private:
    uint previous_;
    uint current_;
    Chunk& chunk_;
    const std::vector<Token>& tokens_;
    ErrorReporter& error_reporter_;
    bool panic_mode_=false;
    ParseRule parse_rules[40];

    void error(const char *message);

};



#endif //CLOXPP_PARSER_HPP
