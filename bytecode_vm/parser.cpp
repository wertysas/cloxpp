//
// Created by Johan Ericsson on 4/24/23.
//

#include "parser.hpp"


// errorAtCurrent -> error_reporter_.report(parser.current, message)
// error -> error_report_.report(parser.previous, message)

// Advances until next non-error token
void Parser::advance() {
    for (uint i = current_+1; i<tokens_.size(); i++) {
        current_ = i;
        if (tokens_[current_].type != TOKEN_ERROR) break;
        error(current_, "Parser Error: NON VALID TOKEN");
    }
}

void Parser::consume(TokenType type, const char* message) {
   if (tokens_[current_].type == type) {
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

void Parser::emit_byte(OpCode opcode) {
    chunk_.add_opcode(opcode, tokens_[current_-1].line);
}
