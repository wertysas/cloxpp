//
// Created by Johan Ericsson on 4/19/23.
//

#include <cstdarg>
#include "common.hpp"
#include "debug.hpp"
#include "binary_operators.hpp"
#include "virtual_machine.hpp"
#include "compiler.hpp"
#include "object.hpp"
#include "value.hpp"

// FIXME 1.
//  Fix proper initialization of objects s.t. only 1 Scanner and Parser are
//  created during lifetime of VM.
//
// FIXME 2.
//  Should accessor of chunk_ be changed to chunk() function in Parser?


InterpretResult VirtualMachine::interpret(const string& source) {
    FunctionObject* function = compile(source);
    if (function == nullptr) {
        return INTERPRET_COMPILE_ERROR;
    }

    stack_.push(Value(function));
    call(function, 0);

    return run( );
}

// Macro for reading bytes with instruction pointer
// we use a macro for readability and to force inlining
#define READ_BYTE( ) (*frame.ip++)
#define READ_CONSTANT( ) \
    (frame.function->chunk.constants[static_cast<uint8_t>(READ_BYTE( ))])
// MACROS for type_ checks 2 force inlining
#define BINARY_NUMBER_CHECK( )                                              \
    do {                                                                    \
        if (!stack_.peek(0).is_number( ) || !stack_.peek(1).is_number( )) { \
            runtime_error("Operands must be numbers.");                     \
            return INTERPRET_RUNTIME_ERROR;                                 \
        }                                                                   \
    } while (false)


InterpretResult VirtualMachine::run( ) {
    CallFrame* current_frame = &frames_[frame_count_ - 1];
    CallFrame frame = *current_frame;

    // switches CallFrame and updates current frame
    auto switch_frame = [&]( ) {
        *current_frame = frame;
        current_frame = &frames_[frame_count_ - 1];
        frame = *current_frame;
    };
    // updates current frame
    auto update_frame = [&]( ) {
        *current_frame = frame;
    };

    for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
        std::cout << "STACK:" << std ::endl;
        for (Value* slot = stack_.top( ); slot > stack_.first( );) {
            std::cout << "[ ";
            print_value(*(--slot));
            std::cout << " ]" << std::endl;
        }
        std::cout << "**********NEXT INSTRUCTION***************" << std ::endl;
        disassemble_instruction(
            frame->function->chunk,
            static_cast<int>(frame->ip -
                             frame->function->chunk.opcodes.head( )));
#endif
        switch (READ_BYTE( )) {
        case OP_CONSTANT: {
            Value constant = READ_CONSTANT( );
            stack_.push(constant);
            break;
        }
        case OP_CONSTANT_LONG: {
            uint32_t const_idx = constant_long_idx(frame.ip);
            frame.ip += 3;
            Value constant = frame.function->chunk.constants[const_idx];
            stack_.push(constant);
            break;
        }
        case OP_NIL:
            stack_.push(Value( ));
            break;
        case OP_TRUE:
            stack_.push(Value(true));
            break;
        case OP_FALSE:
            stack_.push(Value(false));
            break;
        case OP_NOT:
            stack_.push(Value(is_falsy(stack_.pop( ))));
            break;
        case OP_EQUAL:
            //  COMMUTATIVE
            stack_.push(Value(stack_.pop( ) == stack_.pop( )));
            break;
        case OP_NOT_EQUAL:
            //  COMMUTATIVE
            stack_.push(Value(stack_.pop( ) != stack_.pop( )));
            break;
        case OP_GREATER: {
            BINARY_NUMBER_CHECK( );    // only for numbers
            Value v2 = stack_.pop( );
            Value v1 = stack_.pop( );
            stack_.push(Value(v1 > v2));
            break;
        };
        case OP_GREATER_EQUAL: {
            BINARY_NUMBER_CHECK( );    // only for numbers
            Value v2 = stack_.pop( );
            Value v1 = stack_.pop( );
            stack_.push(Value(v1 >= v2));
            break;
        };
        case OP_LESS: {
            BINARY_NUMBER_CHECK( );    // only for numbers
            Value v2 = stack_.pop( );
            Value v1 = stack_.pop( );
            stack_.push(Value(v1 < v2));
            break;
        };
        case OP_LESS_EQUAL: {
            BINARY_NUMBER_CHECK( );    // only for numbers
            Value v2 = stack_.pop( );
            Value v1 = stack_.pop( );
            stack_.push(Value(v1 <= v2));
            break;
        };

        case OP_NEGATE: {
            if (!stack_.peek(0).is_number( )) {
                update_frame( );
                runtime_error("Operand must be a number.");
                return INTERPRET_RUNTIME_ERROR;
            }
            stack_.push(-stack_.pop( ).number_value( ));
            break;
        }
        case OP_POP: {
            stack_.pop( );
            break;
        }
        case OP_POPN: {
            uint8_t n = static_cast<uint8_t>(READ_BYTE( ));
            stack_.popn(n);
            break;
        }
        case OP_GET_LOCAL: {
            uint8_t idx = READ_BYTE( );
            stack_.push(frame.slots[idx]);
            break;
        }
        case OP_GET_LOCAL_LONG: {
            uint32_t idx = constant_long_idx(frame.ip);
            frame.ip += 3;
            stack_.push(frame.slots[idx]);
            break;
        }
        case OP_SET_LOCAL: {
            uint8_t idx = READ_BYTE( );
            frame.slots[idx] = stack_.peek(0);
            break;
        }
        case OP_SET_LOCAL_LONG: {
            uint32_t idx = constant_long_idx(frame.ip);
            frame.ip += 3;
            frame.slots[idx] = stack_.peek(0);
            break;
        }
        case OP_DEFINE_GLOBAL: {
            StringObject* name = READ_CONSTANT( ).string( );
            global_table_.insert(name, stack_.peek(0));
            stack_.pop( );    // late pop, GC related
            break;
        }
        case OP_DEFINE_GLOBAL_LONG: {
            uint32_t const_idx = constant_long_idx(frame.ip);
            frame.ip += 3;
            StringObject* name =
                frame.function->chunk.constants[const_idx].string( );
            global_table_.insert(name, stack_.peek(0));
            stack_.pop( );    // late pop GC related
            break;
        }
        case OP_GET_GLOBAL: {
            StringObject* name = READ_CONSTANT( ).string( );
            entry_type& entry = global_table_.find(name);
            if (entry.type( ) != EntryType::USED) {    // contains
                update_frame( );
                runtime_error("Undefined variable '%s'.", name->chars);
                return INTERPRET_RUNTIME_ERROR;
            }
            stack_.push(entry.value);
            break;
        }
        case OP_GET_GLOBAL_LONG: {
            uint32_t const_idx = constant_long_idx(frame.ip);
            frame.ip += 3;
            StringObject* name =
                frame.function->chunk.constants[const_idx].string( );
            entry_type& entry = global_table_.find(name);
            if (entry.type( ) != EntryType::USED) {
                update_frame( );
                runtime_error("Undefined variable '%s'.", name->chars);
                return INTERPRET_RUNTIME_ERROR;
            }
            stack_.push(entry.value);
            break;
        }
        case OP_SET_GLOBAL: {
            StringObject* name = READ_CONSTANT( ).string( );
            entry_type& entry = global_table_.find(name);
            if (entry.type( ) != EntryType::USED) {
                update_frame( );
                runtime_error("Undefined variable '%s'.", name->chars);
                return INTERPRET_RUNTIME_ERROR;
            }
            entry.value = stack_.peek(0);
            break;
        }
        case OP_SET_GLOBAL_LONG: {
            uint32_t const_idx = constant_long_idx(frame.ip);
            frame.ip += 3;
            StringObject* name =
                frame.function->chunk.constants[const_idx].string( );
            entry_type& entry = global_table_.find(name);
            if (entry.type( ) != EntryType::USED) {
                update_frame( );
                runtime_error("Undefined variable '%s'.", name->chars);
                return INTERPRET_RUNTIME_ERROR;
            }
            entry.value = stack_.peek(0);
            break;
        }
        case OP_ADD: {
            Value v2 = stack_.pop( );
            Value v1 = stack_.pop( );
            if (v1.is_number( ) && v2.is_number( )) {
                stack_.push(v1 + v2);
            } else if (v1.is_string( ) && v2.is_string( )) {
                stack_.push(Value{concatenate(v1.string( ), v2.string( ))});
            } else {
                update_frame( );
                runtime_error("Operands must be two numbers or two strings.");
                return INTERPRET_RUNTIME_ERROR;
            }
            break;
        }
        case OP_SUBTRACT: {
            BINARY_NUMBER_CHECK( );
            Value v2 = stack_.pop( );
            Value v1 = stack_.pop( );
            stack_.push(binary_subtract<Value>(v1, v2));
            break;
        }
        case OP_MULTIPLY: {
            BINARY_NUMBER_CHECK( );
            Value v2 = stack_.pop( );
            Value v1 = stack_.pop( );
            stack_.push(binary_multiply<Value>(v1, v2));
            break;
        }
        case OP_DIVIDE: {
            BINARY_NUMBER_CHECK( );
            // DIVISION  IS NONCOMMUTATIVE, and it's not defined which function
            // in the parameters below is called first. Hence, we introduce
            // local vars stack_.push(binary_divide<Value>(stack_.pop(),
            // stack_.pop()));
            Value denom = stack_.pop( );
            Value num = stack_.pop( );
            stack_.push(binary_divide<Value>(num, denom));
            break;
        }
        case OP_PRINT: {
            print_value(stack_.pop( ));
            std::cout << std::endl;
            break;
        }
        case OP_JUMP: {
            uint16_t offset = twobyte_idx(frame.ip);
            frame.ip += 2;
            frame.ip += offset;
            break;
        }
        case OP_JUMP_IF_FALSE: {
            uint16_t offset = twobyte_idx(frame.ip);
            frame.ip += 2;
            if (is_falsy(stack_.peek(0)))
                frame.ip += offset;
            break;
        }
        case OP_JUMP_IF_TRUE: {
            uint16_t offset = twobyte_idx(frame.ip);
            frame.ip += 2;
            if (!is_falsy(stack_.peek(0)))
                frame.ip += offset;
            break;
        }
        case OP_LOOP: {
            uint16_t offset = twobyte_idx(frame.ip);
            frame.ip += 2;
            frame.ip -= offset;
            break;
        }
        case OP_CALL: {
            uint arg_count = READ_BYTE( );
            update_frame( );
            if (!call_value(stack_.peek(arg_count), arg_count)) {
                return INTERPRET_RUNTIME_ERROR;
            }
            // *current_frame = frame;
            // current_frame = &frames_[frame_count_ - 1];
            // frame = *current_frame;
            switch_frame( );
            break;
        }
        case OP_RETURN: {
            Value result = stack_.pop( );
            frame_count_--;
            if (frame_count_ == 0) {
                stack_.pop( );
                return INTERPRET_SUCCESS;
            }
            stack_.set_top(frame.slots);
            stack_.push(result);

            // *current_frame = frame;
            // current_frame = &frames_[frame_count_ - 1];
            // frame = *current_frame;
            switch_frame( );
            break;
        }
        }
    }
}

bool VirtualMachine::call_value(Value callee, uint arg_count) {
    // Don't want 2 slow down with extra if check
    if (!callee.is_object( )) {
        runtime_error("Can only call functions and classes.");
        return false;
    }
    switch (callee.object_type( )) {
    case OBJ_FUNCTION:
        return call(callee.function( ), arg_count);
    case OBJ_NATIVE: {
        NativeFunction native_function = callee.native_function( );
        Value result = native_function(arg_count, stack_.top( ) - arg_count);
        stack_.set_top(stack_.top( ) - (arg_count + 1));
        stack_.push(result);
        return true;
    }
    default:
        runtime_error("Can only call functions and classes.");
        return false;
    }
}

bool VirtualMachine::call(FunctionObject* function, uint arg_count) {
    if (arg_count != function->arity) {
        runtime_error(
            "Expected %d arguments but got %d.", function->arity, arg_count);
        return false;
    }

    if (frame_count_ == FRAMES_MAX) {
        runtime_error("Stack overflow.");
        return false;
    }

    CallFrame* frame = &frames_[frame_count_++];
    frame->function = function;
    frame->ip = function->chunk.opcodes.head( );
    frame->slots = stack_.top( ) - arg_count - 1;
    return true;
}

void VirtualMachine::runtime_error(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fputs("\n", stderr);


    for (int i = frame_count_ - 1; i >= 0; i--) {
        CallFrame& call_frame = frames_[i];
        FunctionObject* function = call_frame.function;
        size_t offset = call_frame.ip - function->chunk.opcodes.head( );
        uint line = line_number(function->chunk, offset);
        std::cerr << "[line " << line << "] in ";
        if (function->name == nullptr) {
            std::cerr << "script" << std::endl;
        } else {
            std::cerr << function->name->chars << "()" << std::endl;
        }
    }
    stack_.reset( );
}
void VirtualMachine::define_native_function(const char* name,
                                            NativeFunction native_function) {
    stack_.push(Value(str_from_chars(name, static_cast<uint>(strlen(name)))));
    stack_.push(Value(new NativeObject(native_function)));
    global_table_.insert(stack_.peek(1).string( ), stack_.peek(0));
    stack_.pop( );
    stack_.pop( );
}


#undef READ_BYTE
#undef READ_CONSTANT
