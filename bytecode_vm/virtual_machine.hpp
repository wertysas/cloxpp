//
// Created by Johan Ericsson on 4/19/23.
//

#ifndef CLOXPP_VIRTUAL_MACHINE_HPP
#define CLOXPP_VIRTUAL_MACHINE_HPP

#include "chunk.hpp"
#include "stack.hpp"

using ValueStack = StaticStack<Value, 1024>;


enum InterpretResult {
    INTERPRET_SUCCESS,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
};

class VirtualMachine {
    OpCode* ip; // instruction pointer
    Chunk* chunk_;
    ValueStack stack_;
public:
    InterpretResult interpret(Chunk* chunk);
    InterpretResult run();

};


#endif //CLOXPP_VIRTUAL_MACHINE_HPP
