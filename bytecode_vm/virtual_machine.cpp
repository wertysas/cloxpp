//
// Created by Johan Ericsson on 4/19/23.
//

#include <cstdarg>
#include "common.hpp"
#include "debug.hpp"
#include "binary_operators.hpp"
#include "virtual_machine.hpp"

InterpretResult VirtualMachine::interpret(Chunk *chunk) {
    chunk_ = chunk;
    ip = chunk_->opcodes.head();
    return run();
}

// Macro for reading bytes with instruction pointer
// we use a macro for readability and to force inlining
#define READ_BYTE() (*ip++)
#define READ_CONSTANT() (chunk_->constants[static_cast<uint8_t>(READ_BYTE())])
// MACROS for type checks 2 force inlining
#define BINARY_NUMBER_CHECK() \
    do { \
        if (!stack_.peek(0).is_number() || !stack_.peek(1).is_number()) { \
        runtime_error("Numeric binary operation requires operands to be numbers."); \
        return INTERPRET_RUNTIME_ERROR; \
        } \
    } while (false)

InterpretResult VirtualMachine::run() {
    for(;;) {
#ifdef DEBUG_TRACE_EXECUTION
        std::cout << "STACK:" << std ::endl;
        for (Value* slot = stack_.top(); slot>stack_.first();) {
            std::cout << "[ ";
            print_value(*(--slot));
            std::cout << " ]" << std::endl;
        }
        std::cout << "**********NEXT INSTRUCTION***************" << std ::endl;
        disassemble_instruction(*chunk_, static_cast<int>(ip-chunk_->opcodes.head()));
#endif
        switch(READ_BYTE()) {
            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
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
                stack_.push(Value());
                break;
            case OP_TRUE:
                stack_.push(Value(true));
                break;
            case OP_FALSE:
                stack_.push(Value(false));
                break;
            case OP_NOT:
                stack_.push(Value(check_falsiness(stack_.pop())));
                break;
            case OP_EQUAL:
                //  COMMUTATIVE
                stack_.push(Value(stack_.pop()==stack_.pop()));
                break;
            case OP_NOT_EQUAL:
                //  COMMUTATIVE
                stack_.push(Value(stack_.pop()!=stack_.pop()));
                break;
            case OP_GREATER: {
                BINARY_NUMBER_CHECK(); // only for numbers
                Value v2 = stack_.pop();
                Value v1 = stack_.pop();
                stack_.push(Value(v1>v2));
                break;
            };
            case OP_GREATER_EQUAL: {
                BINARY_NUMBER_CHECK(); // only for numbers
                Value v2 = stack_.pop();
                Value v1 = stack_.pop();
                stack_.push(Value(v1>=v2));
                break;
            };
            case OP_LESS: {
                BINARY_NUMBER_CHECK(); // only for numbers
                Value v2 = stack_.pop();
                Value v1 = stack_.pop();
                stack_.push(Value(v1<v2));
                break;
            };
            case OP_LESS_EQUAL: {
                BINARY_NUMBER_CHECK(); // only for numbers
                Value v2 = stack_.pop();
                Value v1 = stack_.pop();
                stack_.push(Value(v1<=v2));
                break;
            };

            case OP_NEGATE: {
                if (!stack_.peek(0).is_number()) {
                    runtime_error("Operand must be a number.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                stack_.push(-stack_.pop().number_value());
                break;
            }
            case OP_ADD: {
                BINARY_NUMBER_CHECK();
                stack_.push(binary_add<Value>(stack_.pop(), stack_.pop()));
                break;
            }
            case OP_SUBTRACT: {
                BINARY_NUMBER_CHECK();
                stack_.push(binary_subtract<Value>(stack_.pop(), stack_.pop()));
                break;
            }
            case OP_MULTIPLY: {
                BINARY_NUMBER_CHECK();
                Value v2 = stack_.pop();
                Value v1 = stack_.pop();
                stack_.push(binary_multiply<Value>(v1, v2));
                //stack_.push(binary_multiply<Value>(stack_.pop(), stack_.pop()));
                break;
            }
            case OP_DIVIDE: {
                BINARY_NUMBER_CHECK();
                // DIVISION  IS NONCOMMUTATIVE and it's not defined which function in
                // the parameters below is called first. Hence we introduce local vars
                // stack_.push(binary_divide<Value>(stack_.pop(), stack_.pop()));
                Value denom = stack_.pop();
                Value num = stack_.pop();
                stack_.push(binary_divide<Value>(num, denom));
                break;
            }
            case OP_RETURN: {
                print_value(stack_.pop());
                std::cout << std::endl;
                return INTERPRET_SUCCESS;
            }
        }
    }
}

void VirtualMachine::runtime_error(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fputs("\n", stderr);

    size_t offset = ip - chunk_->opcodes.head()-1;
    uint line = line_number(*chunk_, offset);
    std::cerr << "[line " << line << "] in script" << std::endl;
    stack_ .reset();
}


#undef READ_BYTE
#undef READ_CONSTANT

