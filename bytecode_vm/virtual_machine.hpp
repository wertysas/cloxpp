//
// Created by Johan Ericsson on 4/19/23.
//

#ifndef CLOXPP_VIRTUAL_MACHINE_HPP
#define CLOXPP_VIRTUAL_MACHINE_HPP

#include "chunk.hpp"
#include "stack.hpp"
#include "error_handling.hpp"
#include "hash_table.hpp"
#include "object.hpp"
#include "common.hpp"


using ValueStack = StaticStack<Value, STACK_MAX>;
enum InterpretResult {
    INTERPRET_SUCCESS,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
};

// CallFrame class holds all info required to handle a function call
struct CallFrame {
    FunctionObject* function; // function executing
    OpCode* ip;
    Value* slots; // pointer to 1st slot in the stack the function can use
};

//template <Size=FRAMES_MAX>
//class CallFrames {
//    CallFrame[]
//};


class VirtualMachine {
    public:
    InterpretResult interpret(const string& source);
    InterpretResult run( );
    VirtualMachine( )
        : stack_( ), global_table_( ) {}

    private:
    using table_type = HashTable<StringObject*, Value, StringHash, StringEqual>;
    using entry_type = TableEntry<StringObject*, Value>;
    ValueStack stack_;
    table_type global_table_;
    CallFrame frames_[FRAMES_MAX];
    uint frame_count_ = 0;
    void runtime_error(const char* fmt...);
    inline CallFrame& current_frame() { return frames_[frame_count_-1]; }
};


#endif    // CLOXPP_VIRTUAL_MACHINE_HPP
