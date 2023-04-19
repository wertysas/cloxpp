//
// Created by Johan Ericsson on 2023-04-04.
//

#include "debug.hpp"



void disassemble_chunk(const Chunk &chunk, std::string name) {
    std::cout << "==" << name << "==" << std::endl;
    for (size_t offset=0; offset<chunk.opcodes.count(); ) {
        offset = disassemble_instruction(chunk, offset);
    }
}


size_t disassemble_instruction(const Chunk &chunk, size_t offset) {
    std::cout << std::right << std::setw(5) << offset << "\t";
    OpCode instruction = chunk.opcodes[offset];
    uint line = line_number(chunk, offset);
    std::cout << std::setw(5) << line << "\t";
    switch(instruction) {
        case OP_RETURN:
            return simple_instruction("OP_RETURN", offset);
        case OP_CONSTANT:
            return constant_instruction(chunk, offset);
        case OP_CONSTANT_LONG:
            return constant_instruction_long(chunk, offset);
        default:
            std::cout << "Unknown opcode " << static_cast<unsigned int>(instruction) << std::endl;
            return offset+1;
    }
}

size_t simple_instruction(std::string str, size_t offset) {
    std::cout << std::setw(16) << std::left << str << std::endl;
    return offset+1;
}

// TODO: ADD VALUE CLASS WITH SPECIFIC PRINT METHOD
size_t constant_instruction(const Chunk &chunk, size_t offset) {
    uint8_t const_idx = chunk.opcodes[offset+1];
    Value val = chunk.constants[const_idx];
    std::cout << std::setw(16) << std::left <<"OP_CONSTANT " << static_cast<unsigned int>(const_idx);
    print_value(val);
    return offset+2;
}

// NOTE:This method makes assumptions of the byte order and only works on little endian machines.
size_t constant_instruction_long(const Chunk &chunk, size_t offset) {
    uint8_t* b = reinterpret_cast<uint8_t*>(&chunk.opcodes[offset+1]);
    uint32_t const_idx = (b[0] | b[1] << 8 | b[2] << 16);
    Value val = chunk.constants[const_idx];
    std::cout << std::left << "OP_CONSTANT_LONG " << static_cast<unsigned int>(const_idx);
    print_value(val);
    return offset+4;
}

uint line_number(const Chunk &chunk, size_t offset) {
    for (int i=0; i<chunk.line_numbers.count(); i++) {
       if (chunk.line_numbers[i].start <= offset && offset < chunk.line_numbers[i].end) {
           return chunk.line_numbers[i].line;
       }
    }
    return 0;
}

