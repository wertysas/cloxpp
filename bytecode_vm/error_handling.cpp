//
// Created by Johan Ericsson on 2023-01-26.
//

#include <iostream>

#include "error_handling.hpp"


void ErrorReporter::error(uint line_nr, const std::string& message) {
    report(line_nr, "", message);
}

void ErrorReporter::report(uint line_nr, const std::string& where, const std::string& message) {
    std::cerr << "[lines " << line_nr << "] Error" << where << ": " << message << std::endl;
    has_error = true;
}

void ErrorReporter::error(const Token &token, const std::string& message) {
    uint line = token.line;
    std::string where;
    if (token.type==TOKEN_EOF) {
        where = " at end";
    } else if (token.type==TOKEN_ERROR) {
        where="";
    } else {
        where = " at " + std::string(token.start, token.length);
    }

    report(line, where, message);
}
