//
// Created by Johan Ericsson on 2023-04-20.
//

#include "compiler.hpp"
#include "scanner.hpp"
#include "parser.hpp"
#include "chunk.hpp"

bool compile(const string &source, Chunk& chunk) {
    Scanner scanner(source);
    Chunk bytecode;
    std::vector tokens = scanner.scan_tokens();
    ErrorReporter error_reporter{};
    Parser parser(tokens, bytecode, error_reporter);
    parser.advance();
//    expression();
//    consume(TOKEN_EOF, "Expect end of expression.");
    bytecode.add_opcode(OP_RETURN, 1000);
    return !parser.had_error();
}
