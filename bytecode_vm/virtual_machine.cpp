//
// Created by Johan Ericsson on 4/19/23.
//

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
InterpretResult VirtualMachine::run() {
    for(;;) {
#ifdef DEBUG_TRACE_EXECUTION
        for (Value* slot = stack_.first(); slot<stack_.top(); slot++) {
            std::cout << "[ " << *slot << " ]" << std::endl;
        }
        disassemble_instruction(*chunk_, static_cast<int>(ip-chunk_->opcodes.head()));
#endif
        OpCode instruction;
        switch(instruction = READ_BYTE()) {
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
            case OP_NEGATE: {
                stack_.push(-stack_.pop());
                break;
            }
            case OP_ADD: {
               stack_.push(binary_add<Value>(stack_.pop(), stack_.pop()));
               break;
            }
            case OP_SUBTRACT: {
                stack_.push(binary_subtract<Value>(stack_.pop(), stack_.pop()));
                break;
            }
            case OP_MULTIPLY: {
                stack_.push(binary_multiply<Value>(stack_.pop(), stack_.pop()));
                break;
            }
            case OP_DIVIDE: {
                stack_.push(binary_divide<Value>(stack_.pop(), stack_.pop()));
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
#undef READ_BYTE
#undef READ_CONSTANT

