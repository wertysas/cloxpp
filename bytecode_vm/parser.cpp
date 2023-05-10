//
// Created by Johan Ericsson on 4/24/23.
//

#include "parser.hpp"

// TODO: MEASURE PERFORMANCE IMPACT OF USING TABLE BASED FUNCTION LOOKUP VS SWITCH/BRANCH
// (IF NEEDED)
Parser::Parser(const std::vector<Token>& tokens,Chunk& chunk, ErrorReporter& error_reporter) :
    previous_(0),
    current_(0),
    chunk_(chunk),
    tokens_(tokens), error_reporter_(error_reporter), parse_rules() {
        parse_rules[TOKEN_LEFT_PAREN]   = {&Parser::grouping,   nullptr,            PREC_NONE};
        parse_rules[TOKEN_RIGHT_PAREN]  = {nullptr,             nullptr,            PREC_NONE};
        parse_rules[TOKEN_LEFT_BRACE]   = {nullptr,             nullptr,            PREC_NONE};
        parse_rules[TOKEN_RIGHT_BRACE]  = {nullptr,             nullptr,            PREC_NONE};
        parse_rules[TOKEN_COMMA]        = {nullptr,             nullptr,            PREC_NONE};
        parse_rules[TOKEN_DOT]          = {nullptr,             nullptr,            PREC_NONE};
        parse_rules[TOKEN_MINUS]        = {&Parser::unary,      &Parser::binary,    PREC_TERM};
        parse_rules[TOKEN_PLUS]         = {nullptr,             &Parser::binary,    PREC_TERM};
        parse_rules[TOKEN_SEMICOLON]    = {nullptr,             nullptr,            PREC_NONE};
        parse_rules[TOKEN_SLASH]        = {nullptr,             &Parser::binary,    PREC_FACTOR};
        parse_rules[TOKEN_STAR]         = {nullptr,             &Parser::binary,    PREC_FACTOR};
        parse_rules[TOKEN_BANG]         = {&Parser::unary,      nullptr,            PREC_NONE};
        parse_rules[TOKEN_BANG_EQUAL]   = {nullptr,             &Parser::binary,    PREC_EQUALITY};
        parse_rules[TOKEN_EQUAL]        = {nullptr,             nullptr,            PREC_NONE};
        parse_rules[TOKEN_EQUAL_EQUAL]  = {nullptr,             &Parser::binary,    PREC_EQUALITY};
        parse_rules[TOKEN_GREATER]      = {nullptr,             &Parser::binary,    PREC_COMPARISON};
        parse_rules[TOKEN_GREATER_EQUAL]= {nullptr,             &Parser::binary,    PREC_COMPARISON};
        parse_rules[TOKEN_LESS]         = {nullptr,             &Parser::binary,    PREC_COMPARISON};
        parse_rules[TOKEN_LESS_EQUAL]   = {nullptr,             &Parser::binary,    PREC_COMPARISON};
        parse_rules[TOKEN_IDENTIFIER]   = {nullptr,             nullptr,            PREC_NONE};
        parse_rules[TOKEN_STRING]       = {&Parser::string,             nullptr,            PREC_NONE};
        parse_rules[TOKEN_NUMBER]       = {&Parser::number,     nullptr,            PREC_NONE};
        parse_rules[TOKEN_AND]          = {nullptr,             nullptr,            PREC_NONE};
        parse_rules[TOKEN_CLASS]        = {nullptr,             nullptr,            PREC_NONE};
        parse_rules[TOKEN_ELSE]         = {nullptr,             nullptr,            PREC_NONE};
        parse_rules[TOKEN_FALSE]        = {&Parser::literal,    nullptr,            PREC_NONE};
        parse_rules[TOKEN_FOR]          = {nullptr,             nullptr,            PREC_NONE};
        parse_rules[TOKEN_FUN]          = {nullptr,             nullptr,            PREC_NONE};
        parse_rules[TOKEN_IF]           = {nullptr,             nullptr,            PREC_NONE};
        parse_rules[TOKEN_NIL]          = {&Parser::literal,    nullptr,            PREC_NONE};
        parse_rules[TOKEN_OR]           = {nullptr,             nullptr,            PREC_NONE};
        parse_rules[TOKEN_PRINT]        = {nullptr,             nullptr,            PREC_NONE};
        parse_rules[TOKEN_RETURN]       = {nullptr,             nullptr,            PREC_NONE};
        parse_rules[TOKEN_SUPER]        = {nullptr,             nullptr,            PREC_NONE};
        parse_rules[TOKEN_THIS]         = {nullptr,             nullptr,            PREC_NONE};
        parse_rules[TOKEN_TRUE]         = {&Parser::literal,    nullptr,            PREC_NONE};
        parse_rules[TOKEN_VAR]          = {nullptr,             nullptr,            PREC_NONE};
        parse_rules[TOKEN_WHILE]        = {nullptr,             nullptr,            PREC_NONE};
        parse_rules[TOKEN_ERROR]        = {nullptr,             nullptr,            PREC_NONE};
        parse_rules[TOKEN_EOF]          = {nullptr,             nullptr,            PREC_NONE};

}

// errorAtCurrent -> error_reporter_.report(parser.current, message)
// error -> error_report_.report(parser.previous, message)
// Advances until next non-error token
void Parser::advance() {
    previous_ = current_;
    for (;;) {
        current_++;
        if (current().type != TOKEN_ERROR) break;
        error(current_, "Parser Error: NON VALID TOKEN");
    }
}


void Parser::consume(TokenType type, const char* message) {
   if (current().type == type) {
       advance();
       return;
   }
    error(current_, message);
}
inline void Parser::error(uint idx, const char* message) {
    if (panic_mode_) return;
    panic_mode_ = true;
    error_reporter_.error(tokens_[idx], message);
}

inline void Parser::error(const char* message) {
    if (panic_mode_) return;
    panic_mode_ = true;
    error_reporter_.error(previous(), message);
}


void Parser::number() {
    Value val{strtod(previous().start, nullptr)};
    chunk_.add_constant(val, previous().line);
}
void Parser::string() {
    Value val{str_from_chars(previous().start+1, previous().length-2)};
    chunk_.add_constant(val, previous().line);
}
void Parser::grouping() {
    expression();
    consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
}

void Parser::unary() {
    TokenType operator_type = previous().type;

    // compile operand
    parse_precedence(PREC_UNARY);

    switch (operator_type) {
        case TOKEN_BANG:
            emit_byte(OP_NOT);
            break;
        case TOKEN_MINUS:
            emit_byte(OP_NEGATE);
            break;
        default:
            return;
    }
}

void Parser::parse_precedence(Precedence precedence) {
    advance();
    ParseFn prefix_rule = parse_rule(previous().type)->prefix;
    if (prefix_rule == nullptr) {
        error("Expect expression. in parse precedence");
        return;
    }

    (this->*prefix_rule)();

    while (precedence <= parse_rule(current().type)->precedence) {
        advance();
        ParseFn infix_rule = parse_rule(previous().type)->infix;
        if (infix_rule == nullptr) {
            error("Expected non nullptr in parse precedence function");
            return;
        }
        (this->*infix_rule)();
    }
}

void Parser::expression() {
    parse_precedence(PREC_ASSIGNMENT);
}

void Parser::binary() {
    TokenType operator_type = previous().type;
    ParseRule* rule = parse_rule(operator_type);
    parse_precedence(static_cast<Precedence>(rule->precedence+1));
    switch (operator_type) {
        case TOKEN_BANG_EQUAL:
            emit_byte(OP_NOT_EQUAL);
            break;
        case TOKEN_EQUAL_EQUAL:
            emit_byte(OP_EQUAL);
            break;
        case TOKEN_GREATER:
            emit_byte(OP_GREATER);
            break;
        case TOKEN_GREATER_EQUAL:
            emit_byte(OP_GREATER_EQUAL);
            break;
        case TOKEN_LESS:
            emit_byte(OP_LESS);
            break;
        case TOKEN_LESS_EQUAL:
            emit_byte(OP_LESS_EQUAL);
            break;
        case TOKEN_PLUS:
            emit_byte(OP_ADD);
            break;
        case TOKEN_MINUS:
            emit_byte(OP_SUBTRACT);
            break;
        case TOKEN_STAR:
            emit_byte(OP_MULTIPLY);
            break;
        case TOKEN_SLASH:
            emit_byte(OP_DIVIDE);
            break;
        default:
            return;
    }
}

void Parser::literal() {
    switch(previous().type) {
        case TOKEN_FALSE:
            emit_byte(OP_FALSE);
            break;
        case TOKEN_NIL:
            emit_byte(OP_NIL);
            break;
        case TOKEN_TRUE:
            emit_byte(OP_TRUE);
            break;
        default:
            return;
    }
}


inline ParseRule* Parser::parse_rule(TokenType type) {
    return &parse_rules[type];
}

inline void Parser::emit_byte(OpCode opcode) {
    chunk_.add_opcode(opcode, tokens_[current_-1].line);
}

inline void Parser::emit_byte(uint idx, OpCode opcode) {
    chunk_.add_opcode(opcode, tokens_[idx].line);
}

void Parser::emit_double_byte(OpCode op1, OpCode op2) {
    chunk_.add_opcode(op1, tokens_[current_-1].line);
    chunk_.add_opcode(op2, tokens_[current_-1].line);
}



