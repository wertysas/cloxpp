//
// Created by Johan Ericsson on 2023-04-20.
//

#ifndef CLOXPP_FUNCTION_SCOPE_HPP
#define CLOXPP_FUNCTION_SCOPE_HPP

#include "common.hpp"
#include "token.hpp"
#include "static_array.hpp"
#include "object.hpp"



struct LocalVariable {
    short depth = -1;
    bool captured = false;
    Token token;
};

struct UpValue {
    uint8_t idx;
    bool is_local;
};

enum class FunctionType { FUNCTION, SCRIPT };

struct FunctionScope {
    uint16_t local_count;
    uint16_t scope_depth;
    FunctionScope* enclosing;
    FunctionObject* function;
    FunctionType type;
    StaticArray<LocalVariable, UINT8_MAX + 1> locals;
    StaticArray<UpValue, UINT8_MAX + 1> upvalues;

    FunctionScope( )
        : local_count(0), scope_depth(0), enclosing(nullptr),
          type(FunctionType::SCRIPT), locals{ } {
        function = new FunctionObject;
        LocalVariable& local = locals[local_count++];
        local.depth = 0;
        local.captured = false;
        local.token.start = "";
        local.token.length = 0;
    }

    FunctionScope(FunctionScope* enclosing_, StringObject* name)
        : local_count(0), scope_depth(0), enclosing(enclosing_),
          type(FunctionType::FUNCTION), locals{ } {
        memory::temporary_roots.push_back(name);
        function = new FunctionObject(name);
        memory::temporary_roots.pop_back();
        memory::temporary_roots.pop_back(); // push_back called in operator new for FunctionObject
        LocalVariable& local = locals[local_count++];
        local.depth = 0;
        local.captured = false;
        local.token.start = "";
        local.token.length = 0;
    }

    constexpr LocalVariable& operator[](size_t index) {
        return locals.begin( )[index];
    }

    constexpr LocalVariable& last( ) { return operator[](local_count - 1); }

    bool add_local(Token name) {
        if (local_count == UINT8_MAX + 1) {
            return false;
        }
        LocalVariable& local = locals[local_count++];
        local.token = name;
        local.depth = -1;
        local.captured = false;
        return true;
    }

    void mark_initialized( ) {
        if (scope_depth == 0)
            return;
        locals[local_count - 1].depth = scope_depth;
    }
};



#endif    // CLOXPP_FUNCTION_SCOPE_HPP
