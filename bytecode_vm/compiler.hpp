//
// Created by Johan Ericsson on 2023-04-20.
//

#ifndef CLOXPP_COMPILER_HPP
#define CLOXPP_COMPILER_HPP

#include "common.hpp"
#include "chunk.hpp"
#include "token.hpp"
#include "static_array.hpp"
#include "object.hpp"


struct LocalVariable {
    short depth = -1;
    Token token;
};

enum class FunctionType { FUNCTION, SCRIPT };
struct FunctionScope {
    uint8_t local_count;
    uint8_t scope_depth;
    FunctionScope* enclosing;
    FunctionObject* function;
    FunctionType type;
    StaticArray<LocalVariable, UINT8_MAX + 1> locals;

    FunctionScope( )
        : local_count(0), scope_depth(0), enclosing(nullptr),
          type(FunctionType::SCRIPT), locals{ } {
        function = new FunctionObject;
    }
    FunctionScope(FunctionScope* enclosing_, FunctionType type_)
        : local_count(0), scope_depth(0), enclosing(enclosing_),
          type(type_), locals{ } {
        function = new FunctionObject;
        LocalVariable& local = locals[local_count++];
        local.depth = 0;
        local.token.start = "";
        local.token.length = 0;
    }
    constexpr LocalVariable& operator[](size_t index) {
        return locals.begin( )[index];
    }
    constexpr LocalVariable& last( ) { return operator[](local_count - 1); }
    bool add_local(Token name) {
        if (local_count == UINT8_MAX) {
            return false;
        }
        LocalVariable& local = locals[local_count++];
        local.token = name;
        local.depth = -1;
        return true;
    }
    void mark_initialized( ) {
        if (scope_depth == 0)
            return;
        locals[local_count - 1].depth = scope_depth;
    }
};

FunctionObject* compile(const string& source);


#endif    // CLOXPP_COMPILER_HPP
