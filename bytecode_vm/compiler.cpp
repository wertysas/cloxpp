//
// Created by Johan Ericsson on 2023-09-26.
//

#include "compiler.hpp"

#if defined DEBUG_PRINT_CODE || defined DEBUG_PRINT_TOKENS
#include "debug.hpp"
#endif

FunctionObject* Compiler::compile(const string& source) {
        Scanner scanner(source);
        std::vector<Token> tokens = scanner.scan_tokens();
        ErrorReporter error_reporter{};
        FunctionScope main_scope{};
#ifdef DEBUG_PRINT_TOKENS
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
