//
// Created by Johan Ericsson on 2023-04-20.
//

#include "scanner.hpp"
#include "../treewalk_interpreter/scanner.hpp"

Scanner::Scanner(const string &source) : start_(source.c_str()), current_(source.c_str()), line_(1) {}

Token Scanner::next_token() {
    handle_white_space();
    start_ = current_;
    if (end_of_string()) {
        return Token(TOKEN_EOF, start_, static_cast<uint>(current_ - start_), line_);
    }

    char c = advance();
    if (is_alpha())
    if (is_digit(c)) return handle_numeral();

    switch (c) {
        case '(':
            return Token(TOKEN_LEFT_PAREN, start_, static_cast<uint>(current_-start_), line_);
        case ')':
            return Token(TOKEN_RIGHT_PAREN, start_, static_cast<uint>(current_-start_), line_);
        case '{':
            return Token(TOKEN_LEFT_BRACE, start_, static_cast<uint>(current_-start_), line_);
        case '}':
            return Token(TOKEN_RIGHT_BRACE, start_, static_cast<uint>(current_-start_), line_);
        case ';':
            return Token(TOKEN_SEMICOLON, start_, static_cast<uint>(current_-start_), line_);
        case ',':
            return Token(TOKEN_COMMA, start_, static_cast<uint>(current_-start_), line_);
        case '.':
            return Token(TOKEN_DOT, start_, static_cast<uint>(current_-start_), line_);
        case '-':
            return Token(TOKEN_MINUS, start_, static_cast<uint>(current_-start_), line_);
        case '+':
            return Token(TOKEN_PLUS, start_, static_cast<uint>(current_-start_), line_);
        case '/':
            return Token(TOKEN_SLASH, start_, static_cast<uint>(current_-start_), line_);
        case '*':
            return Token(TOKEN_STAR, start_, static_cast<uint>(current_-start_), line_);

        // two character punctuation tokens
        case '!':
            return Token(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG, start_, static_cast<uint>(current_-start_), line_);
        case '=':
            return Token(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL, start_, static_cast<uint>(current_-start_), line_);
        case '<':
            return Token(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS, start_, static_cast<uint>(current_-start_), line_);
        case '>':
            return Token(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER, start_, static_cast<uint>(current_-start_), line_);
        case '"':
            return handle_string();
    }
    // TODO: FIX ERROR REPORTING FOR ERROR TOKEN (MAYBE INCLUDE ERROR REPORTER)
    return Token(TOKEN_ERROR, start_, static_cast<uint>(current_-start_), line_);
}

bool Scanner::match(char expected) {
    if (end_of_string()) return false;
    if (*current_ != expected) return false;
    current_++;
    return true;
}

void Scanner::handle_white_space() {
    for (;;) {
        char c = peek();
        switch(c) {
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            case '\n':
                line_++;
                advance();
                break;
            case '/':
                // comment
                if (peek2() == '/') {
                    while (peek() != '\n' && !end_of_string()) { advance(); }
                } else {
                    return;
                }
                break;
            default:
                return;
        }
    }
}

Token Scanner::handle_string() {
    while  (peek() != '"' && !end_of_string()) {
        if (peek() == '\n') { line_++; }
        advance();
    }
    if (end_of_string()) {
        return errortoken("Unterminated string.");
    }
    advance();
    return token(TOKEN_STRING);
}

Token Scanner::handle_numeral() {
    while (is_digit(peek())) advance();
y
    if (peek()=='.' && is_digit(peek2())) {
        advance();
        while (is_digit(peek())) advance();
    }
    return token(TOKEN_NUMBER);
}

Token Scanner::handle_identifier() {
    while (is_alpha(peek()) || is_digit(peek())) advance();
    return token(identifier_token());
}

inline Token Scanner::token(TokenType type) {
    return Token(type, start_, static_cast<uint>(current_-start_), line_);
}

// FIXME: fix error reporting
inline Token Scanner::errortoken(const char* error_msg) {
    return Token(TOKEN_ERROR, start_, static_cast<uint>(current_-start_), line_);
}


inline char Scanner::peek() {
    return *current_;
}

inline char Scanner::peek2() {
    if (end_of_string()) return '\0';
    return current_[1];
}

inline char Scanner::advance() {
    current_++;
    return current_[-1];
}

inline bool Scanner::end_of_string() {
    return *current_ == '\0';
}

inline bool Scanner::is_digit(char c) {
    return  ('0'<= c && c<='9');
}
inline bool Scanner::is_alpha(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_';
}


TokenType Scanner::identifier_token() {
    switch (*start_) {
        case 'a': return check_keyword(3, "nd", TOKEN_AND);
    }
    return TOKEN_IDENTIFIER;
}

inline bool Scanner::check_keyword(uint start, uint len, const char* keyword) {
   if ()
}