//
// Created by Johan Ericsson on 2023-04-20.
//

#ifndef CLOXPP_SCANNER_HPP
#define CLOXPP_SCANNER_HPP

#include "common.hpp"
#include "token.hpp"

class Scanner {
    public:
        Scanner(const string& source);
        Token next_token();
    private:
        const char* start_;
        const char* current_;
        uint line_;

    char advance();
    bool match(char expected);
    char peek();
    void handle_white_space();
    char peek2();
    bool end_of_string();

    Token handle_string();

    Token token(TokenType type);

    Token errortoken(const char *error_msg);

    bool is_digit(char c);

    Token handle_numeral();

    bool is_alpha(char c);

    Token handle_identifier();

    TokenType identifier_token();

    TokenType check_keyword(uint len, const char *keyword, TokenType type);
};


#endif //CLOXPP_SCANNER_HPP
