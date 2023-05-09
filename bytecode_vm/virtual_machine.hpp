//
// Created by Johan Ericsson on 4/19/23.
//

#ifndef CLOXPP_VIRTUAL_MACHINE_HPP
#define CLOXPP_VIRTUAL_MACHINE_HPP

#include "chunk.hpp"
#include "stack.hpp"
#include "error_handling.hpp"

using ValueStack = StaticStack<Value, 1024>;


enum InterpretResult {
    INTERPRET_SUCCESS,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
};

class VirtualMachine {
public:
    InterpretResult interpret(Chunk* chunk);
    InterpretResult run();
    //VirtualMachine(Chunk* chunk) : chunk_(chunk), ip(chunk->opcodes.head()), stack_() {}
private:
    OpCode* ip; // instruction pointer
    Chunk* chunk_;
    ValueStack stack_;
    void runtime_error(const char* fmt...);
};


#endif //CLOXPP_VIRTUAL_MACHINE_HPP
