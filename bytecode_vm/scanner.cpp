//
// Created by Johan Ericsson on 2023-04-20.
//

#include "scanner.hpp"

Scanner::Scanner(const string &source) : start_(source.c_str()), current_(source.c_str()), line_(1) {}

Token Scanner::scanToken() {
    start_ = current_;
    if (end_of_file()) {
        return Token(TOKEN_EOF, start_, static_cast<uint>(current_-start_), line_);
    }
    // TODO: FIX ERROR REPORTING FOR ERROR TOKEN (MAYBE INCLUDE ERROR REPORTER)
    return Token(TOKEN_ERROR, start_, static_cast<uint>(current_-start_), line_);

    char c = advance();
}

bool Scanner::end_of_file() {
    return *current_ == '\0';
}

