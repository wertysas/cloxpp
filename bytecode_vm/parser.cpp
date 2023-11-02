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
    : previous_(0), current_(0), scope_(scope), class_scope_(nullptr), tokens_(tokens),
      error_reporter_(error_reporter), parse_rules( ) {
    parse_rules[TOKEN_LEFT_PAREN] = {
        &Parser::grouping, &Parser::call, PREC_CALL};
    parse_rules[TOKEN_RIGHT_PAREN] = {nullptr, nullptr, PREC_NONE};
    parse_rules[TOKEN_LEFT_BRACE] = {nullptr, nullptr, PREC_NONE};
    parse_rules[TOKEN_RIGHT_BRACE] = {nullptr, nullptr, PREC_NONE};
    parse_rules[TOKEN_COMMA] = {nullptr, nullptr, PREC_NONE};
    parse_rules[TOKEN_DOT] = {nullptr, &Parser::dot, PREC_CALL};
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
    parse_rules[TOKEN_THIS] = {&Parser::this_, nullptr, PREC_NONE};
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
    Value val{new StringObject(previous( ).start + 1, previous( ).length - 2)};
    chunk( ).add_constant(val, previous( ).line);
}

void Parser::grouping(bool assignable) {
    expression( );
    consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
}

void Parser::call(bool assignable) {
    uint8_t arg_count = argument_list( );
    emit_byte(OP_CALL);
    emit_byte(static_cast<OpCode>(arg_count));
}

void Parser::dot(bool assignable) {
    consume(TOKEN_IDENTIFIER, "Expect property name after '.'.");
    uint name_idx = identifier_constant(previous( ));

    if (assignable && match(TOKEN_EQUAL)) {
        expression( );
        emit_byte_with_index(OP_SET_PROPERTY, OP_SET_PROPERTY_LONG, name_idx);
    } else {
        emit_byte_with_index(OP_GET_PROPERTY, OP_GET_PROPERTY_LONG, name_idx);
    }
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

void Parser::this_(bool asignable) {
    if (class_scope_ == nullptr) {
        error("Can't use 'this' outside of a class.");
        return;
    }
    variable(false);
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

inline void Parser::emit_byte(OpCode opcode, uint8_t idx) {
    chunk( ).add_opcode(opcode, tokens_[current_ - 1].line);
    chunk( ).add_opcode(static_cast<OpCode>(idx), tokens_[current_ - 1].line);
}

uint Parser::emit_constant(Value value) {
    chunk( ).constants.append(value);
    return chunk( ).constants.count( ) - 1;
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
    if (match(TOKEN_CLASS)) {
        class_declaration( );
    } else if (match(TOKEN_FUN)) {
        function_declaration( );
    } else if (match(TOKEN_VAR)) {
        var_declaration( );
    } else {
        statement( );
    }
    if (panic_mode_)
        synchronize( );
}

void Parser::class_declaration( ) {
    consume(TOKEN_IDENTIFIER, "Expect class name.");
    Token class_name = previous();
    uint idx = identifier_constant(previous( ));
    declare_variable( );
    emit_byte_with_index(OP_CLASS, OP_CLASS_LONG, idx);

    define_variable(idx);

    ClassScope class_scope;
    class_scope.enclosing_scope() = class_scope_;
    class_scope_ = &class_scope;
    named_variable(class_name, false);
    consume(TOKEN_LEFT_BRACE, "Expect '{' before class body.");
    while (!check(TOKEN_RIGHT_BRACE) && !check(TOKEN_EOF)) {
        method( );
    }
    consume(TOKEN_RIGHT_BRACE, "Expect '}' after class body.");
    emit_byte(OP_POP);

    class_scope_ = class_scope_->enclosing_scope();
}
void Parser::function_declaration( ) {
    uint8_t global = parse_variable("Expect function name.");
    scope_->mark_initialized( );
    function(FunctionType::FUNCTION);
    define_variable(global);
}

void Parser::function(FunctionType type) {
    StringObject* name =
        new StringObject(previous( ).start, previous( ).length);
    FunctionScope function_scope(scope_, name, type);
    update_scope(&function_scope);
    begin_scope( );    // no end_scope() since scope lifetime only is inside
                       // function

    consume(TOKEN_LEFT_PAREN, "Expect '(' after function name.");
    // function parameters
    if (!check(TOKEN_RIGHT_PAREN)) {
        do {
            scope_->function->arity++;
            if (scope_->function->arity > 255) {
                error(current_, "Can't have more than 255 parameters.");
            }
            uint param_constant_idx = parse_variable("Expect parameter name.");
            define_variable(param_constant_idx);
        } while (match(TOKEN_COMMA));
    }
    consume(TOKEN_RIGHT_PAREN, "Expect ')' after parameters.");
    consume(TOKEN_LEFT_BRACE, "Expect '{' before function body.");
    block( );

    FunctionObject* function = close_function_scope( );
    memory::temporary_roots.push_back(function);
    uint idx = emit_constant(Value(function));
    memory::temporary_roots.pop_back( );
    emit_byte_with_index(OP_CLOSURE, OP_CLOSURE_LONG, idx);

    for (uint i = 0; i < function->upvalue_count; i++) {
        UpValue& upvalue = function_scope.upvalues[i];
        // std::cout << "idx: " << upvalue.idx << " is_local: " <<
        // upvalue.is_local << std::endl;
        emit_byte(upvalue.is_local ? static_cast<OpCode>(1)
                                   : static_cast<OpCode>(0));
        emit_byte(static_cast<OpCode>(upvalue.idx));
    }
}

void Parser::method( ) {
    consume(TOKEN_IDENTIFIER, "Expect method name.");
    uint idx = identifier_constant(previous( ));
    if (idx >= 255) {
        error("Can't declare class methods inside chunk with more than 255 "
              "opcodes.");
    }
    function(FunctionType::METHOD);
    emit_byte(OP_METHOD, idx);
}

uint8_t Parser::argument_list( ) {
    uint8_t arg_count = 0;
    if (!check(TOKEN_RIGHT_PAREN)) {
        do {
            expression( );
            if (arg_count == 255) {
                error("Can't have more than 255 arguments.");
            }
            arg_count++;
        } while (match(TOKEN_COMMA));
    }
    consume(TOKEN_RIGHT_PAREN, "Expect ')' after parameters.");
    return arg_count;
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
    } else if (match(TOKEN_RETURN)) {
        return_statement( );
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

void Parser::return_statement( ) {
    if (scope_->type == FunctionType::SCRIPT) {
        error("Can't return from top-level code.");
    }
    if (match(TOKEN_SEMICOLON)) {
        emit_return( );
    } else {
        expression( );
        consume(TOKEN_SEMICOLON, "Expect ';' after return value;");
        emit_byte(OP_RETURN);
    }
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
    return chunk( ).add_constant(
        Value(new StringObject(token.start, token.length)));
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
        error("Too many local variables in function.");
        return;
    }
}

void Parser::emit_byte_with_index(OpCode op_normal, OpCode op_long, uint idx) {
    if (idx < UINT8_MAX) {
        chunk( ).add_opcode(op_normal, tokens_[current_ - 1].line);
        emit_byte(static_cast<OpCode>(idx));
    }
    // Else we assume that idx fits in 24 bits
    else {
        chunk( ).add_opcode(op_long, tokens_[current_ - 1].line);
        OpCode* indices = reinterpret_cast<OpCode*>(&idx);
        emit_byte(indices[0]);
        emit_byte(indices[1]);
        emit_byte(indices[2]);
    }
}

uint Parser::emit_jump(OpCode opcode) {
    emit_byte(opcode);
    emit_byte(static_cast<OpCode>(0xff));
    emit_byte(static_cast<OpCode>(0xff));
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
        if (scope_->locals[scope_->local_count - 1].captured) {
            if (n > 0) {
                emit_byte(OP_POPN);
                emit_byte(static_cast<OpCode>(n));
            }
            n = 0;
            emit_byte(OP_CLOSE_UPVALUE);
        } else {
            ++n;
        }
        --scope_->local_count;
    }
    if (n > 0) {
        emit_byte(OP_POPN);
        emit_byte(static_cast<OpCode>(n));
    }
}

FunctionObject* Parser::close_function_scope( ) {
    emit_return( );
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
    uint idx = resolve_local(*scope_, token);
    OpCode set_op, set_op_long, get_op, get_op_long;
    if (idx != UINT_MAX) {
        // TODO: These can all be set by 1 operation check if the compiler
        // implements that optimization!
        set_op = OP_SET_LOCAL;
        set_op_long = OP_SET_LOCAL_LONG;
        get_op = OP_GET_LOCAL;
        get_op_long = OP_GET_LOCAL_LONG;
    } else if ((idx = resolve_upvalue(*scope_, token)) != UINT_MAX) {
        set_op = OP_SET_UPVALUE;
        set_op_long = OP_SET_UPVALUE_LONG;
        get_op = OP_GET_UPVALUE;
        get_op_long = OP_GET_UPVALUE_LONG;
    } else {
        idx = identifier_constant(token);
        set_op = OP_SET_GLOBAL;
        set_op_long = OP_SET_GLOBAL_LONG;
        get_op = OP_GET_GLOBAL;
        get_op_long = OP_GET_GLOBAL_LONG;
    }
    if (assignable && match(TOKEN_EQUAL)) {
        expression( );
        emit_byte_with_index(set_op, set_op_long, idx);
    } else {
        emit_byte_with_index(get_op, get_op_long, idx);
    }
}

uint Parser::resolve_local(FunctionScope& scope, const Token& token) {
    for (uint i = scope.local_count; i > 0; i--) {    // 0-1 = UINT_MAX
        LocalVariable& local = scope[i - 1];
        if (lexemes_equal(token, local.token)) {
            if (local.depth == -1) {
                error("Can't read local variable in its own initializer.");
                return UINT_MAX;
            }
            return i - 1;
        }
    }
    return UINT_MAX;
}

uint Parser::resolve_upvalue(FunctionScope& scope, const Token& token) {
    if (scope.enclosing == nullptr)
        return UINT_MAX;

    uint local = resolve_local(*scope.enclosing, token);
    if (local != UINT_MAX) {
        scope_->enclosing->locals[local].captured = true;
        return add_upvalue(scope, local, true);
    }

    uint upvalue = resolve_upvalue(*scope.enclosing, token);
    if (upvalue != UINT_MAX) {
        return add_upvalue(scope, upvalue, false);
    }

    return UINT_MAX;
}

uint Parser::add_upvalue(FunctionScope& scope, uint idx, bool is_local) {
    uint16_t upvalue_count = scope.function->upvalue_count;

    for (uint i = 0; i < upvalue_count; i++) {
        UpValue& upvalue = scope.upvalues[i];
        if (upvalue.idx == idx && upvalue.is_local == is_local) {
            return i;
        }
    }

    if (upvalue_count == UINT8_MAX + 1) {
        error("Too many closure variables in function.");
        return 0;
    }

    scope.upvalues[upvalue_count].is_local = is_local;
    scope.upvalues[upvalue_count].idx = idx;
    return scope.function->upvalue_count++;
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
    emit_byte(jumps[0]);
    emit_byte(jumps[1]);
}
void Parser::emit_return( ) {
    emit_byte(OP_NIL);
    emit_byte(OP_RETURN);
}

void Parser::mark_compiler_roots( ) {
    auto* scope = scope_;
    while (scope != nullptr) {
        memory::mark_object(scope->function);
        scope = scope->enclosing;
    }
}
