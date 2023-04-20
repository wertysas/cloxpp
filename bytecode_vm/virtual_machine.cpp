//
// Created by Johan Ericsson on 4/19/23.
//

#include "common.hpp"
#include "debug.hpp"
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
//#ifdef DEBUG_TRACE_EXECUTION
        disassemble_instruction(*chunk_, static_cast<int>(ip-chunk_->opcodes.head()));
        for (Value* slot = stack_.first(); slot<stack_.top(); slot++) {
            std::cout << "[ " << *slot << " ]" << std::endl;
        }
//#endif
        OpCode instruction;
        switch(instruction = READ_BYTE()) {
            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
                stack_.push(constant);
                //std::cout << constant << std::endl;
                break;
            }
            case OP_CONSTANT_LONG: {
                uint32_t const_idx = constant_long_idx(ip);
                ip += 3;
                Value constant = chunk_->constants[const_idx];
                stack_.push(constant);
                //std::cout << constant << std::endl;
                break;
            }
            case OP_RETURN: {
                return INTERPRET_SUCCESS;
            }
        }
    }
}
#undef READ_BYTE
#undef READ_CONSTANT

