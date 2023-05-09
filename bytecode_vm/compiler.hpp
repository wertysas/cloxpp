//
// Created by Johan Ericsson on 2023-04-20.
//

#ifndef CLOXPP_COMPILER_HPP
#define CLOXPP_COMPILER_HPP

#include "common.hpp"
#include "chunk.hpp"

bool compile(const string &source, Chunk& bytecode);


#endif //CLOXPP_COMPILER_HPP
