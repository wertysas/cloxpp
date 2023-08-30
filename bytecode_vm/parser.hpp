//
// Created by Johan Ericsson on 4/24/23.
//

#ifndef CLOXPP_PARSER_HPP
#define CLOXPP_PARSER_HPP


#include "common.hpp"
#include "token.hpp"
#include "error_handling.hpp"
#include "chunk.hpp"
#include "compiler.hpp"


// Precedence enum where the different presedences are in order of importance
// thus PREC_X < PREC_Y if PREC_Y has presedence over PREC_X
enum Precedence : uint8_t {
    PREC_NONE,
    PREC_ASSIGNMENT,    // =
    PREC_OR,            // or
    PREC_AND,           // and
    PREC_EQUALITY,      // == !=
    PREC_COMPARISON,    // < > <= >=
    PREC_TERM,          // + -
    PREC_FACTOR,        // * /
    PREC_UNARY,         // ! -
    PREC_CALL,          // . ()
    PREC_PRIMARY
};

class Parser;
typedef void (Parser::*ParseFn)(bool);

struct ParseRule {
    ParseFn prefix;
    ParseFn infix;
    Precedence precedence;
};

class Parser {
    public:
    Parser(const std::vector<Token>& tokens,
           FunctionScope* scope,
           ErrorReporter& error_reporter);

    void advance( );
    void consume(TokenType type, const char* message);
    bool match(TokenType type);
    bool check(TokenType type);
    inline Chunk& chunk( ) const { return scope_->function->chunk; }

    FunctionObject* parse_tokens( );

    // Error handling methods
    void error(uint idx, const char* message);
    bool had_error( ) const { return error_reporter_.has_error; };
    void synchronize( );

    inline const Token& previous( ) const { return tokens_[previous_]; }
    inline const Token& current( ) const { return tokens_[current_]; }

    // Pars Functions
    void number(bool assignable);
    void string(bool assignable);
    void grouping(bool assignable);
    void call(bool assignable);
    void variable(bool assignable);
    void literal(bool assignable);
    void unary(bool assignable);
    void binary(bool assignable);
    void and_(bool assignable);
    void or_(bool assignable);

    void expression( );
    void parse_precedence(Precedence precedence);
    void declaration( );
    void function_declaration( );
    void function();
    uint8_t argument_list();
    void var_declaration( );
    void statement( );
    void if_statement( );
    void return_statement();
    void while_statement( );
    void for_statement( );
    void print_statement( );
    void expression_statement( );
    void block( );
    void begin_scope( );
    void end_scope( );
    inline void update_scope(FunctionScope* fn_scope) { scope_ = fn_scope; };
    FunctionObject* close_function_scope( );
    uint parse_variable(const char* error_msg);
    void define_variable(uint global);
    void declare_variable( );
    void named_variable(const Token& token, bool assignable);
    uint resolve_local(FunctionScope& scope, const Token& token);
    uint resolve_upvalue(FunctionScope& scope, const Token& token);
    uint add_upvalue(FunctionScope& scope, uint idx, bool is_local);
    uint identifier_constant(Token const& token);
    ParseRule* parse_rule(TokenType type);

    void emit_byte(OpCode opcode); // adds OpCode to chunk of function in scope
    void emit_byte(uint token_idx, OpCode opcode);
    uint emit_constant(Value value);
    void emit_byte_with_index(OpCode op_short, OpCode op_long, uint idx);
    uint emit_jump(OpCode opcode);
    void patch_jump(uint offset);
    void emit_loop(uint loop_start);
    void emit_return();

    private:
    uint previous_;
    uint current_;
    FunctionScope* scope_;
    const std::vector<Token>& tokens_;
    ErrorReporter& error_reporter_;
    bool panic_mode_ = false;
    ParseRule parse_rules[40];

    void error(const char* message);
};


#endif    // CLOXPP_PARSER_HPP
