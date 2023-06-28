//
// Created by Johan Ericsson on 2023-04-20.
//

#ifndef CLOXPP_COMPILER_HPP
#define CLOXPP_COMPILER_HPP

#include "common.hpp"
#include "chunk.hpp"
#include "token.hpp"
#include "static_array.hpp"

struct LocalVariable {
    short depth=-1;
    Token token;
};

struct Scope {
    uint8_t local_count;
    uint8_t scope_depth;
    StaticArray<LocalVariable, UINT8_MAX+1> locals;
    constexpr LocalVariable& operator[](size_t index) {
        return locals.begin()[index];
    }
    constexpr LocalVariable& last() {
        return operator[](local_count-1);
    }
    bool add_local(Token name) {
        if (local_count==UINT8_MAX+1) {
            return false;
        }
        LocalVariable local = locals[local_count++];
        local.token = name;
        local.depth = scope_depth;
        return true;
    }
    void mark_initialized() {
        locals[local_count-1].depth=scope_depth;
    }
};

bool compile(const string &source, Chunk& bytecode);


#endif //CLOXPP_COMPILER_HPP
