//
// Created by Johan Ericsson on 4/24/23.
//

#include <climits>
#include "parser.hpp"
#include "debug.hpp"

// TODO: MEASURE PERFORMANCE IMPACT OF USING TABLE BASED FUNCTION LOOKUP VS
// SWITCH/BRANCH (IF NEEDED)
Parser::Parser(const std::vector<Token>& tokens,
               FunctionScope* scope,
               ErrorReporter& error_reporter)
    : previous_(0), current_(0), tokens_(tokens), scope_(scope),
      error_reporter_(error_reporter), parse_rules( ) {
    parse_rules[TOKEN_LEFT_PAREN] = {&Parser::grouping, nullptr, PREC_NONE};
    parse_rules[TOKEN_RIGHT_PAREN] = {nullptr, nullptr, PREC_NONE};
    parse_rules[TOKEN_LEFT_BRACE] = {nullptr, nullptr, PREC_NONE};
    parse_rules[TOKEN_RIGHT_BRACE] = {nullptr, nullptr, PREC_NONE};
    parse_rules[TOKEN_COMMA] = {nullptr, nullptr, PREC_NONE};
    parse_rules[TOKEN_DOT] = {nullptr, nullptr, PREC_NONE};
    parse_rules[TOKEN_MINUS] = {&Parser::unary, &Parser::binary, PREC_TERM};
    parse_rules[TOKEN_PLUS] = {nullptr, &Parser::binary, PREC_TERM};
    parse_rules[TOKEN_SEMICOLON] = {nullptr, nullptr, PREC_NONE};
    parse_rules[TOKEN_SLASH] = {nullptr, &Parser::binary, PREC_FACTOR};
    parse_rules[TOKEN_STAR] = {nullptr, &Parser::binary, PREC_FACTOR};
    parse_rules[TOKEN_BANG] = {&Parser::unary, nullptr, PREC_NONE};
    parse_rules[TOKEN_BANG_EQUAL] = {nullptr, &Parser::binary, PREC_EQUALITY};
    parse_rules[TOKEN_EQUAL] = {nullptr, nullptr, PREC_NONE};
    parse_rules[TOKEN_EQUAL_EQUAL] = {nullptr, &Parser::binary, PREC_EQUALITY};
    parse_rules[TOKEN_GREATER] = {nullptr, &Parser::binary, PREC_COMPARISON};
    parse_rules[TOKEN_GREATER_EQUAL] = {
        nullptr, &Parser::binary, PREC_COMPARISON};
    parse_rules[TOKEN_LESS] = {nullptr, &Parser::binary, PREC_COMPARISON};
    parse_rules[TOKEN_LESS_EQUAL] = {nullptr, &Parser::binary, PREC_COMPARISON};
    parse_rules[TOKEN_IDENTIFIER] = {&Parser::variable, nullptr, PREC_NONE};
    parse_rules[TOKEN_STRING] = {&Parser::string, nullptr, PREC_NONE};
    parse_rules[TOKEN_NUMBER] = {&Parser::number, nullptr, PREC_NONE};
    parse_rules[TOKEN_AND] = {nullptr, &Parser::and_, PREC_AND};
    parse_rules[TOKEN_CLASS] = {nullptr, nullptr, PREC_NONE};
    parse_rules[TOKEN_ELSE] = {nullptr, nullptr, PREC_NONE};
    parse_rules[TOKEN_FALSE] = {&Parser::literal, nullptr, PREC_NONE};
    parse_rules[TOKEN_FOR] = {nullptr, nullptr, PREC_NONE};
    parse_rules[TOKEN_FUN] = {nullptr, nullptr, PREC_NONE};
    parse_rules[TOKEN_IF] = {nullptr, nullptr, PREC_NONE};
    parse_rules[TOKEN_NIL] = {&Parser::literal, nullptr, PREC_NONE};
    parse_rules[TOKEN_OR] = {nullptr, &Parser::or_, PREC_OR};
    parse_rules[TOKEN_PRINT] = {nullptr, nullptr, PREC_NONE};
    parse_rules[TOKEN_RETURN] = {nullptr, nullptr, PREC_NONE};
    parse_rules[TOKEN_SUPER] = {nullptr, nullptr, PREC_NONE};
    parse_rules[TOKEN_THIS] = {nullptr, nullptr, PREC_NONE};
    parse_rules[TOKEN_TRUE] = {&Parser::literal, nullptr, PREC_NONE};
    parse_rules[TOKEN_VAR] = {nullptr, nullptr, PREC_NONE};
    parse_rules[TOKEN_WHILE] = {nullptr, nullptr, PREC_NONE};
    parse_rules[TOKEN_ERROR] = {nullptr, nullptr, PREC_NONE};
    parse_rules[TOKEN_EOF] = {nullptr, nullptr, PREC_NONE};
}

// errorAtCurrent -> error_reporter_.report(parser.current, message)
// error -> error_report_.report(parser.previous, message)
// Advances until next non-error token
void Parser::advance( ) {
    previous_ = current_;
    for (;;) {
        current_++;
        if (current( ).type != TOKEN_ERROR)
            break;
        // if (current_ >= tokens_.size( )) {
        //     error(current_, "Out of token range while parsing");
        // }
        error(current_, current( ).start);
    }
}


void Parser::consume(TokenType type, const char* message) {
    if (current( ).type == type) {
        advance( );
        return;
    }
    error(current_, message);
}
inline void Parser::error(uint idx, const char* message) {
    if (panic_mode_)
        return;
    panic_mode_ = true;
    error_reporter_.error(tokens_[idx], message);
}

inline void Parser::error(const char* message) {
    if (panic_mode_)
        return;
    panic_mode_ = true;
    error_reporter_.error(previous( ), message);
}

void Parser::number(bool assignable) {
    Value val{strtod(previous( ).start, nullptr)};
    chunk( ).add_constant(val, previous( ).line);
}

void Parser::string(bool assignable) {
    Value val{str_from_chars(previous( ).start + 1, previous( ).length - 2)};
    chunk( ).add_constant(val, previous( ).line);
}

void Parser::grouping(bool assignable) {
    expression( );
    consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
}

void Parser::literal(bool assignable) {
    switch (previous( ).type) {
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

void Parser::variable(bool assignable) {
    named_variable(previous( ), assignable);
}

void Parser::unary(bool assignable) {
    TokenType operator_type = previous( ).type;

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

void Parser::binary(bool assignable) {
    TokenType operator_type = previous( ).type;
    ParseRule* rule = parse_rule(operator_type);
    parse_precedence(static_cast<Precedence>(rule->precedence + 1));
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

// maybe remove nullptr checks if this is hot code
void Parser::parse_precedence(Precedence precedence) {
    advance( );
    ParseFn prefix_rule = parse_rule(previous( ).type)->prefix;
    if (prefix_rule == nullptr) {
        error("Expect expression.");
        return;
    }
    bool assignable = precedence <= PREC_ASSIGNMENT;
    (this->*prefix_rule)(assignable);

    while (precedence <= parse_rule(current( ).type)->precedence) {
        advance( );
        ParseFn infix_rule = parse_rule(previous( ).type)->infix;
        if (infix_rule == nullptr) {
            error("Expected non nullptr in parse precedence function");
            return;
        }
        (this->*infix_rule)(assignable);
    }

    if (assignable && match(TOKEN_EQUAL)) {
        error("Invalid assignment target.");
    }
}

uint Parser::parse_variable(const char* error_msg) {
    consume(TOKEN_IDENTIFIER, error_msg);

    declare_variable( );
    if (scope_->scope_depth > 0)
        return 0;


    return identifier_constant(previous( ));
}

void Parser::expression( ) {
    parse_precedence(PREC_ASSIGNMENT);
}


inline ParseRule* Parser::parse_rule(TokenType type) {
    return &parse_rules[type];
}

inline void Parser::emit_byte(OpCode opcode) {
    chunk( ).add_opcode(opcode, tokens_[current_ - 1].line);
}

inline void Parser::emit_byte(uint idx, OpCode opcode) {
    chunk( ).add_opcode(opcode, tokens_[idx].line);
}

FunctionObject* Parser::parse_tokens( ) {
    if (current( ).type == TOKEN_ERROR) {
        error(current_, current( ).start);
    }
    while (!match(TOKEN_EOF)) {
        declaration( );
    }

    return close_function_scope( );
}

bool Parser::match(TokenType type) {
    if (!(current( ).type == type))
        return false;
    advance( );
    return true;
}

inline bool Parser::check(TokenType type) {
    return current( ).type == type;
}
void Parser::declaration( ) {
    if (match(TOKEN_FUN)) {
        function_declaration( );
    } else if (match(TOKEN_VAR)) {
        var_declaration( );
    } else {
        statement( );
    }
    if (panic_mode_)
        synchronize( );
}

void Parser::function_declaration( ) {
    uint8_t global = parse_variable("Expect function name.");
    scope_->mark_initialized( );
    function(FunctionType::FUNCTION);
    define_variable(global);
}

void Parser::function(FunctionType fun_type) {
    FunctionScope function_scope(scope_, fun_type);
    update_scope(&function_scope);
    begin_scope( );    // no end_scope() since scope lifetime only is inside
                       // function

    consume(TOKEN_LEFT_PAREN, "Expect '(' after function name.");
    consume(TOKEN_LEFT_PAREN, "Expect ')' after parameters.");
    consume(TOKEN_LEFT_BRACE, "Expect '{' before function body.");
    block( );

    FunctionObject* function = close_function_scope( );
    chunk().add_constant(Value(function), previous().line);
}

void Parser::var_declaration( ) {
    uint global = parse_variable("Expect variable name.");

    if (match(TOKEN_EQUAL)) {
        expression( );
    } else {
        emit_byte(OP_NIL);
    }
    consume(TOKEN_SEMICOLON, "Expect ';' after variable declaration.");

    define_variable(global);
}

void Parser::statement( ) {
    if (match(TOKEN_PRINT)) {
        print_statement( );
    } else if (match(TOKEN_IF)) {
        if_statement( );
    } else if (match(TOKEN_WHILE)) {
        while_statement( );
    } else if (match(TOKEN_FOR)) {
        for_statement( );
    } else if (match(TOKEN_LEFT_BRACE)) {
        begin_scope( );
        block( );
        end_scope( );
    } else {
        expression_statement( );
    }
}

void Parser::if_statement( ) {
    consume(TOKEN_LEFT_PAREN, "Expect '(' after 'if'.");
    expression( );    // adds a falsy value to the stack
    consume(TOKEN_RIGHT_PAREN, "Expect '(' after 'if'.");

    uint then_jump =
        emit_jump(OP_JUMP_IF_FALSE);    // if true jump to after else
    emit_byte(OP_POP);                  // pops falsy value
    statement( );                       // executes statement if not false

    uint else_jump = emit_jump(OP_JUMP);
    patch_jump(then_jump);    // sets if-false jump to correct location
    emit_byte(OP_POP);        // pops truthy value

    if (match(TOKEN_ELSE)) {    // conditionally executes else statement
        statement( );
    }
    patch_jump(else_jump);    // sets else jump to correct location
}

void Parser::while_statement( ) {
    uint loop_start = chunk( ).opcodes.count( );
    consume(TOKEN_LEFT_PAREN, "Expect '(' after 'while'.");
    expression( );
    consume(TOKEN_RIGHT_PAREN, "Expect ')' after condition.");

    uint exit_jump = emit_jump(OP_JUMP_IF_FALSE);
    emit_byte(OP_POP);
    statement( );
    emit_loop(loop_start);


    patch_jump(exit_jump);
    emit_byte(OP_POP);
}

void Parser::for_statement( ) {
    begin_scope( );
    consume(TOKEN_LEFT_PAREN, "Expect '(' after for.");
    // Initializer clause
    if (match(TOKEN_SEMICOLON)) {
        // Empty first control stmt (no initializer)
    } else if (match(TOKEN_VAR)) {
        var_declaration( );
    } else {
        expression_statement( );
    }

    uint loop_start = chunk( ).opcodes.count( );
    int exit_jump = -1;
    // Condition clause
    if (!match(TOKEN_SEMICOLON)) {
        expression( );
        consume(TOKEN_SEMICOLON, "Expect ';' after loop condition");
        // break loop if condition is false
        exit_jump = emit_jump(OP_JUMP_IF_FALSE);
        emit_byte(OP_POP);    // pop condition value
    }

    // Increment clause
    if (!match(TOKEN_RIGHT_PAREN)) {
        uint body_jump = emit_jump(OP_JUMP);
        uint increment_start = chunk( ).opcodes.count( );
        expression( );
        emit_byte(OP_POP);
        consume(TOKEN_RIGHT_PAREN, "Expect ')' after for clauses.");

        emit_loop(loop_start);
        loop_start = increment_start;
        patch_jump(body_jump);
    }

    statement( );
    emit_loop(loop_start);
    if (exit_jump != -1) {
        patch_jump(exit_jump);
        emit_byte(OP_POP);    // pop condition value
    }
    end_scope( );
}

void Parser::print_statement( ) {
    expression( );
    consume(TOKEN_SEMICOLON, "Expect ';' after value.");
    emit_byte(OP_PRINT);
}

void Parser::expression_statement( ) {
    expression( );
    consume(TOKEN_SEMICOLON, "Expect ';' after expression.");
    emit_byte(OP_POP);
}

void Parser::synchronize( ) {
    panic_mode_ = false;

    while (current( ).type != TOKEN_EOF) {
        if (previous( ).type == TOKEN_SEMICOLON)
            return;
        switch (current( ).type) {
        case TOKEN_CLASS:
        case TOKEN_FUN:
        case TOKEN_VAR:
        case TOKEN_FOR:
        case TOKEN_IF:
        case TOKEN_WHILE:
        case TOKEN_PRINT:
        case TOKEN_RETURN:
            return;
        default:
            advance( );
        }
    }
}
uint Parser::identifier_constant(const Token& token) {
    return chunk( ).constants.idx_append(
        Value(str_from_chars(token.start, token.length)));
}
void Parser::define_variable(uint idx) {
    if (scope_->scope_depth > 0) {
        scope_->mark_initialized( );
        return;
    }
    emit_byte_with_index(OP_DEFINE_GLOBAL, OP_DEFINE_GLOBAL_LONG, idx);
}

void Parser::declare_variable( ) {
    if (scope_->scope_depth == 0)
        return;

    Token name = previous( );
    for (int i = static_cast<int>(scope_->local_count) - 1; i >= 0; i--) {
        LocalVariable local = scope_->locals[i];
        if (local.depth != -1 && local.depth < scope_->scope_depth) {
            break;
        }

        if (lexemes_equal(name, local.token)) {
            error("Already a variable with this name in this scope.");
        }
    }


    if (!scope_->add_local(name)) {
        error("Too many local variables declared in scope (maximum is 256).");
        return;
    }
}

void Parser::emit_byte_with_index(OpCode op_normal, OpCode op_long, uint idx) {
    if (idx < UINT8_MAX) {
        chunk( ).add_opcode(op_normal, tokens_[current_ - 1].line);
        chunk( ).opcodes.append(static_cast<OpCode>(idx));
    }
    // Else we assume that idx fits in 24 bits
    else {
        chunk( ).add_opcode(op_long, tokens_[current_ - 1].line);
        OpCode* indices = reinterpret_cast<OpCode*>(&idx);
        chunk( ).opcodes.append(indices[0]);
        chunk( ).opcodes.append(indices[1]);
        chunk( ).opcodes.append(indices[2]);
    }
}

uint Parser::emit_jump(OpCode opcode) {
    emit_byte(opcode);
    chunk( ).opcodes.append(static_cast<OpCode>(0xff));
    chunk( ).opcodes.append(static_cast<OpCode>(0xff));
    return chunk( ).opcodes.count( ) - 2;
}

void Parser::patch_jump(uint offset) {
    uint jump = chunk( ).opcodes.count( ) - offset - 2;
    if (jump > UINT16_MAX) {
        error("Too much code to jump over.");
    }

    OpCode* jumps = reinterpret_cast<OpCode*>(&jump);
    chunk( ).opcodes[offset] = jumps[0];
    chunk( ).opcodes[offset + 1] = jumps[1];
}

void Parser::block( ) {
    while (!check(TOKEN_RIGHT_BRACE) && !check(TOKEN_EOF)) {
        declaration( );
    }
    consume(TOKEN_RIGHT_BRACE, "Expect '}' after block.");
}

void Parser::begin_scope( ) {
    ++scope_->scope_depth;
}

void Parser::end_scope( ) {
    --scope_->scope_depth;
    // while
    uint n = 0;
    while (scope_->local_count > 0 &&
           scope_->last( ).depth > scope_->scope_depth) {
        ++n;
        --scope_->local_count;
    }
    emit_byte(OP_POPN);
    emit_byte(static_cast<OpCode>(n));
}

FunctionObject* Parser::close_function_scope( ) {
    emit_byte(OP_RETURN);
    FunctionObject* function_ptr = scope_->function;
#ifdef DEBUG_PRINT_CODE
    if (!had_error( )) {
        disassemble_chunk(function_ptr->chunk,
                          function_ptr->name != nullptr
                              ? function_ptr->name->chars
                              : "<script>");
    }
#endif
    scope_ = scope_->enclosing;
    return function_ptr;
}

void Parser::named_variable(const Token& token, bool assignable) {
    uint idx = resolve_local(token);
    if (idx != UINT_MAX) {
        if (assignable && match(TOKEN_EQUAL)) {
            expression( );
            emit_byte_with_index(OP_SET_LOCAL, OP_SET_LOCAL_LONG, idx);
        } else {
            emit_byte_with_index(OP_GET_LOCAL, OP_GET_LOCAL_LONG, idx);
        }
    } else {
        idx = identifier_constant(token);
        if (assignable && match(TOKEN_EQUAL)) {
            expression( );
            emit_byte_with_index(OP_SET_GLOBAL, OP_SET_GLOBAL_LONG, idx);
        } else {
            emit_byte_with_index(OP_GET_GLOBAL, OP_GET_GLOBAL_LONG, idx);
        }
    }
}

uint Parser::resolve_local(const Token& token) {
    for (uint i = scope_->local_count; i > 0; i--) {
        LocalVariable& local = (*scope_)[i - 1];
        if (lexemes_equal(token, local.token)) {
            if (local.depth == -1) {
                error("Can't read local variable in its own initializer.");
            }
            return i - 1;
        }
    }
    return UINT_MAX;
}
void Parser::and_(bool assignable) {
    uint end_jump = emit_jump(OP_JUMP_IF_FALSE);
    // Left value of and_ expression is already on stack and must be popped
    emit_byte(OP_POP);             // pop left-hand side if true
    parse_precedence(PREC_AND);    // parse right-hand side of expression
    patch_jump(end_jump);    // set jump to after second part of and_ expression
                             // (leaves falsy value on stack)
}
void Parser::or_(bool assignable) {
    uint end_jump = emit_jump(OP_JUMP_IF_TRUE);
    emit_byte(OP_POP);
    parse_precedence(PREC_OR);
    patch_jump(end_jump);
}
void Parser::emit_loop(uint loop_start) {
    emit_byte(OP_LOOP);
    uint offset = chunk( ).opcodes.count( ) - loop_start + 2;
    if (offset > UINT16_MAX)
        error("Loop body too large.");

    OpCode* jumps = reinterpret_cast<OpCode*>(&offset);
    chunk( ).opcodes.append(jumps[0]);
    chunk( ).opcodes.append(jumps[1]);
}
Chunk& Parser::chunk( ) {
    return scope_->function->chunk;
}
