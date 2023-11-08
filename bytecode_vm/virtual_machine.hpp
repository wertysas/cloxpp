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
#include "cpp_module.hpp"



enum InterpretResult {
    INTERPRET_SUCCESS,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
};

// CallFrame class holds all info required to handle a function call
struct CallFrame {
    ClosureObject* closure; // function executing
    OpCode* ip;
    Value* slots; // pointer to 1st slot in the stack the function can use
};


class VirtualMachine {
    public:
    InterpretResult interpret(FunctionObject* function);
    InterpretResult run( );
    VirtualMachine( )
        : stack_( ), global_table_( ), init_string_(nullptr) {
        define_native_function("clock", native_clock);
        init_string_ = new StringObject("init", 4);
    }

    void mark_init_string();
    void mark_globals();
    void mark_stack();
    void mark_call_frames();
    void mark_upvalues();

        private:
    using table_type = HashTable<StringObject*, Value, StringHash, StringEqual>;
    using entry_type = TableEntry<StringObject*, Value>;
    using value_stack = StaticStack<Value, STACK_MAX>;
    value_stack stack_;
    table_type global_table_;
    CallFrame frames_[FRAMES_MAX];
    uint frame_count_ = 0;
    UpValueObject* open_upvalues_ = nullptr;
    StringObject* init_string_;

    inline CallFrame& current_frame() { return frames_[frame_count_-1]; }

    UpValueObject* capture_upvalue(Value* local_value);
    void close_upvalues(Value* last);

    bool call_value(Value callee, uint arg_count);
    bool invoke(StringObject* name, uint8_t arg_count);
    bool invoke_from_class(ClassObject* klas, StringObject* name, uint8_t arg_count);
    bool call(ClosureObject* closure, uint arg_count);
    void define_method(StringObject* name);
    bool bind_method(ClassObject* klass, StringObject* name);
    void define_native_function(const char* name, NativeFunction native_function);
    void runtime_error(const char* fmt...);
};


#endif    // CLOXPP_VIRTUAL_MACHINE_HPP
