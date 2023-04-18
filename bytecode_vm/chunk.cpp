//
// Created by Johan Ericsson on 2023-04-03.
//

#include "chunk.hpp"


Chunk::Chunk() : opcodes(), constants(), line_numbers() {
    line_numbers.append(LineInfo());
}


void Chunk::add_opcode(OpCode opcode, uint line) {
    opcodes.append(opcode);
    if (line_numbers[line_numbers.count()-1].line < line) {
        line_numbers.append(LineInfo(0, line));
    } else {
       line_numbers[line_numbers.count()-1].count++;
    }
}

void Chunk::add_constant(Value constant) {
    uint32_t idx = constants.idx_append(constant);
    if (idx < UINT8_MAX) {
        opcodes.append(OP_CONSTANT);
        opcodes.append(static_cast<OpCode>(idx));
    }
    // Else we assume that idx fits in 24 bits
    else {
        opcodes.append(OP_CONSTANT_LONG);
        OpCode* indices = reinterpret_cast<OpCode*>(&idx);
        opcodes.append(indices[0]);
        opcodes.append(indices[1]);
        opcodes.append(indices[2]);
    }

}




