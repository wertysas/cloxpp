//
// Created by Johan Ericsson on 2023-04-20.
//

#include "compiler.hpp"
#include "scanner.hpp"
#include "parser.hpp"
#include "chunk.hpp"

#ifdef DEBUG_PRINT_CODE
#include "debug.hpp"
#endif



bool compile(const string &source, Chunk& bytecode) {
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.scan_tokens();
    ErrorReporter error_reporter{};
    Scope current_scope;
#ifdef DEBUG_PRINT_CODE
    std::cout << "tokens size: " << tokens.size() << std::endl;
    for (auto t: tokens) {
        std::cout << "TokenType: " << token_to_string(t.type) << std::endl;
    }
#endif
    Parser parser(tokens, bytecode, current_scope, error_reporter);
    //parser.advance();
    parser.parse_tokens();
    //parser.consume(TOKEN_EOF, "Expect end of expression.");
    bytecode.add_opcode(OP_RETURN, 1000);

    // end_compiler()
#ifdef DEBUG_PRINT_CODE
    if (!parser.had_error()) {
        disassemble_chunk(bytecode, "code");
    }
#endif
    return !parser.had_error();
}
