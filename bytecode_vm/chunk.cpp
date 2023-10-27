//
// Created by Johan Ericsson on 2023-04-03.
//

#include "chunk.hpp"

Chunk::Chunk() : constants(), opcodes(), line_numbers() {
}

Value* Chunk::tmp_value = nullptr;

void Chunk::add_opcode(OpCode opcode, uint line) {
    opcodes.append(opcode);
    if (line_numbers.count()==0 || line_numbers.back().line < line) {
        line_numbers.append(LineInfo(line, opcodes.count()-1));
    } else {
       line_numbers.back().end = opcodes.count()-1;
    }
}


uint Chunk::add_constant(Value constant) {
    tmp_value = &constant;
    uint32_t idx = constants.idx_append(constant);
    tmp_value = nullptr;
    return idx;

}

uint Chunk::add_constant(Value constant, uint line) {
    tmp_value = &constant;
    uint32_t idx = constants.idx_append(constant);
    if (idx < UINT8_MAX) {
        add_opcode(OP_CONSTANT, line);
        opcodes.append(static_cast<OpCode>(idx));
    }
    // Else we assume that idx fits in 24 bits
    else {
        add_opcode(OP_CONSTANT_LONG, line);
        OpCode* indices = reinterpret_cast<OpCode*>(&idx);
        opcodes.append(indices[0]);
        opcodes.append(indices[1]);
        opcodes.append(indices[2]);
    }
    tmp_value = nullptr;
    return idx;
}


void Chunk::mark_constants( ) {
    for (uint i=0; i<constants.count(); i++) {
        constants[i].mark();
    }
    if (tmp_value != nullptr) {
        tmp_value->mark();
    }
}
