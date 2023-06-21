//
// Created by Johan Ericsson on 4/19/23.
//

#ifndef CLOXPP_VIRTUAL_MACHINE_HPP
#define CLOXPP_VIRTUAL_MACHINE_HPP

#include "chunk.hpp"
#include "stack.hpp"
#include "error_handling.hpp"
#include "hash_table.hpp"

using ValueStack = StaticStack<Value, 1024>;

enum InterpretResult {
    INTERPRET_SUCCESS,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
};

class VirtualMachine {
    public:
    InterpretResult interpret(Chunk* chunk);
    InterpretResult run( );
    VirtualMachine( )
        : chunk_(nullptr), ip(nullptr), stack_( ), global_table_( ) {}

    private:
    struct StringEqual {
        constexpr bool operator()(StringObject* s1, StringObject* s2) { return true; }
    };
    using table_type = HashTable<StringObject*, Value, StringHash, StringEqual>;
    using entry_type = TableEntry<StringObject*, Value>;
    OpCode* ip;    // instruction pointer
    Chunk* chunk_;
    ValueStack stack_;
    table_type global_table_;
    void runtime_error(const char* fmt...);
};


#endif    // CLOXPP_VIRTUAL_MACHINE_HPP
