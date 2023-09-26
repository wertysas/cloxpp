//
// Created by Johan Ericsson on 2023-09-26.
//

#ifndef CLOXPP_COMPILER_HPP
#define CLOXPP_COMPILER_HPP

#include "parser.hpp"
#include "scanner.hpp"

class Compiler {
    // Scanner& scanner_;
    // Parser& parser_;
    public:
    FunctionObject* compile(const string& source);

};


#endif    // CLOXPP_COMPILER_HPP
