//
// Created by Johan Ericsson on 2023-01-26.
//

#ifndef CLOXPP_SCANNER_HPP
#define CLOXPP_SCANNER_HPP


#include <string>
#include <utility>
#include <vector>
#include "token.hpp"
#include "error_handling.hpp"


class Scanner {
    public:
        Scanner(std::string source_, ErrorReporter &errorReporter_) :
            source(std::move(source_)), errorReporter(errorReporter_) {}
        std::vector<Token> scanTokens();
    private:
        int start=0, current=0, line=1;
        string source;
        std::vector<Token> tokens;
        ErrorReporter& errorReporter;
        void scanToken();
        char advance();
        void addToken(TokenType type);
        void addToken(TokenType type, void* literal);
        bool match(char c);
        char peek() const;
        inline bool end_of_source() const { return current >= source.length(); }

    void handle_string();

    void handle_number();

    bool is_digit(char peek);

    char peek_next() const;

    bool is_letter(char c);
};


#endif //CLOXPP_SCANNER_HPP
