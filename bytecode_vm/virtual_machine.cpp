//
// Created by Johan Ericsson on 4/19/23.
//

#include <cstdarg>
#include "common.hpp"
#include "debug.hpp"
#include "binary_operators.hpp"
#include "virtual_machine.hpp"

InterpretResult VirtualMachine::interpret(Chunk* chunk) {
    chunk_ = chunk;
    ip = chunk_->opcodes.head( );
    return run( );
}

// Macro for reading bytes with instruction pointer
// we use a macro for readability and to force inlining
#define READ_BYTE( ) (*ip++)
#define READ_CONSTANT( ) (chunk_->constants[static_cast<uint8_t>(READ_BYTE( ))])
// MACROS for type_ checks 2 force inlining
#define BINARY_NUMBER_CHECK( )                                                \
    do {                                                                      \
        if (!stack_.peek(0).is_number( ) || !stack_.peek(1).is_number( )) {   \
            runtime_error(                                                    \
                "Operands must be numbers."); \
            return INTERPRET_RUNTIME_ERROR;                                   \
        }                                                                     \
    } while (false)


InterpretResult VirtualMachine::run( ) {
    for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
        std::cout << "STACK:" << std ::endl;
        for (Value* slot = stack_.top( ); slot > stack_.first( );) {
            std::cout << "[ ";
            print_value(*(--slot));
            std::cout << " ]" << std::endl;
        }
        std::cout << "**********NEXT INSTRUCTION***************" << std ::endl;
        disassemble_instruction(*chunk_,
                                static_cast<int>(ip - chunk_->opcodes.head( )));
#endif
        switch (READ_BYTE( )) {
        case OP_CONSTANT: {
            Value constant = READ_CONSTANT( );
            stack_.push(constant);
            break;
        }
        case OP_CONSTANT_LONG: {
            uint32_t const_idx = constant_long_idx(ip);
            ip += 3;
            Value constant = chunk_->constants[const_idx];
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
            uint8_t n = static_cast<uint8_t>(READ_BYTE());
            stack_.popn(n);
            break;
        }
        case OP_GET_LOCAL: {
            uint8_t idx = READ_BYTE( );
            stack_.push(stack_[idx]);
            break;
        }
        case OP_GET_LOCAL_LONG: {
            uint32_t idx = constant_long_idx(ip);
            ip += 3;
            stack_.push(stack_[idx]);
            break;
        }
        case OP_SET_LOCAL: {
            uint8_t idx = READ_BYTE( );
            stack_[idx] = stack_.peek(0);
            break;
        }
        case OP_SET_LOCAL_LONG: {
            uint32_t idx = constant_long_idx(ip);
            ip += 3;
            stack_[idx] = stack_.peek(0);
            break;
        }
        case OP_DEFINE_GLOBAL: {
            StringObject* name = READ_CONSTANT( ).string( );
            global_table_.insert(name, stack_.peek(0));
            stack_.pop( );    // late pop, GC related
            break;
        }
        case OP_DEFINE_GLOBAL_LONG: {
            uint32_t const_idx = constant_long_idx(ip);
            ip += 3;
            StringObject* name = chunk_->constants[const_idx].string( );
            global_table_.insert(name, stack_.peek(0));
            stack_.pop( );    // late pop GC related
            break;
        }
        case OP_GET_GLOBAL: {
            StringObject* name = READ_CONSTANT( ).string( );
            entry_type& entry = global_table_.find(name);
            if (entry.type( ) != EntryType::USED) { // contains
                runtime_error("Undefined variable '%s'.", name->chars);
                return INTERPRET_RUNTIME_ERROR;
            }
            stack_.push(entry.value);
            break;
        }
        case OP_GET_GLOBAL_LONG: {
            uint32_t const_idx = constant_long_idx(ip);
            ip += 3;
            StringObject* name = chunk_->constants[const_idx].string( );
            entry_type& entry = global_table_.find(name);
            if (entry.type( ) != EntryType::USED) {
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
                runtime_error("Undefined variable '%s'.", name->chars);
                return INTERPRET_RUNTIME_ERROR;
            }
            entry.value = stack_.peek(0);
            break;
        }
        case OP_SET_GLOBAL_LONG: {
            uint32_t const_idx = constant_long_idx(ip);
            ip += 3;
            StringObject* name = chunk_->constants[const_idx].string( );
            entry_type& entry = global_table_.find(name);
            if (entry.type( ) != EntryType::USED) {
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
            } else if (v1.is_string( ) &&
                       v2.is_string( )) {
                stack_.push(Value{concatenate(v1.string( ), v2.string( ))});
            } else {
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
            // stack_.push(binary_multiply<Value>(stack_.pop(), stack_.pop()));
            break;
        }
        case OP_DIVIDE: {
            BINARY_NUMBER_CHECK( );
            // DIVISION  IS NONCOMMUTATIVE, and it's not defined which function
            // in the parameters below is called first. Hence, we introduce local
            // vars stack_.push(binary_divide<Value>(stack_.pop(),
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
            uint16_t offset = twobyte_idx(ip);
            ip += 2;
            ip += offset;
            break;
        }
        case OP_JUMP_IF_FALSE: {
            uint16_t offset = twobyte_idx(ip);
            ip +=2;
            if (is_falsy(stack_.peek(0)))
                ip += offset;
            break;
        }
        case OP_JUMP_IF_TRUE: {
            uint16_t offset = twobyte_idx(ip);
            ip += 2;
            if (!is_falsy(stack_.peek(0)))
                ip += offset;
            break;
        }
        case OP_LOOP: {
            uint16_t offset = twobyte_idx(ip);
            ip += 2;
            ip -= offset;
            break;
        }
        case OP_RETURN: {
            return INTERPRET_SUCCESS;
        }
        }
    }
}

void VirtualMachine::runtime_error(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fputs("\n", stderr);

    size_t offset = ip - chunk_->opcodes.head( ) - 1;
    uint line = line_number(*chunk_, offset);
    std::cerr << "[line " << line << "] in script" << std::endl;
    stack_.reset( );
}


#undef READ_BYTE
#undef READ_CONSTANT
