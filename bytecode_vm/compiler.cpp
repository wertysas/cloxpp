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



FunctionObject* compile(const string &source) {
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.scan_tokens();
    ErrorReporter error_reporter{};
    FunctionScope main_scope{};
#ifdef DEBUG_PRINT_CODE
    std::cout << "tokens size: " << tokens.size() << std::endl;
    for (auto t: tokens) {
        std::cout << "TokenType: " << token_to_string(t.type) << std::endl;
    }
#endif
    Parser parser(tokens, &main_scope, error_reporter);
    //parser.advance();
    FunctionObject* function = parser.parse_tokens();
    //parser.consume(TOKEN_EOF, "Expect end of expression.");

    return parser.had_error() ? nullptr : function;
}
