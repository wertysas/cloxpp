//
// Created by Johan Ericsson on 2023-04-04.
//

#include "debug.hpp"


void disassemble_chunk(Chunk& chunk, std::string name) {
    std::cout << "==" << name << "==" << std::endl;
    for (size_t offset=0; offset<chunk.count(); ) {
        offset = disassemble_instruction(chunk, offset);
    }
}

size_t disassemble_instruction(Chunk& chunk, size_t offset) {
    std::cout << offset << " ";
    OpCode instruction = chunk[offset];
    switch(instruction) {
        case OP_RETURN:
            return simple_instruction("OP_RETURN", offset);
        default:
            std::cout << "Unkown opcode" << instruction << std::endl;
            return offset+1;
    }
}

size_t simple_instruction(std::string str, size_t offset) {
    std::cout << str << std::endl;
    return offset+1;
}
