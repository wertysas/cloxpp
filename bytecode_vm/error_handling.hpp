//
// Created by Johan Ericsson on 2023-01-26.
//

#ifndef CLOXPP_ERROR_HANDLING_HPP
#define CLOXPP_ERROR_HANDLING_HPP

#include <string>
#include <stdexcept>
#include "common.hpp"
#include "token.hpp"


class ParserError : public std::runtime_error {
    public:
    explicit ParserError(const char* message) : std::runtime_error(message) {}
    explicit ParserError(const string& message) : std::runtime_error(message) {}

};

class ErrorReporter {
    public:
        bool has_error=false;
        void error(uint line_nr, const std::string& message);
        void error(const Token& token, const std::string& message);
    private:
        void report(uint line_nr, const std::string& where, const std::string& message);

};



#endif //CLOXPP_ERROR_HANDLING_HPP
