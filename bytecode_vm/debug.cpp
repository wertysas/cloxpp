//
// Created by Johan Ericsson on 2023-04-04.
//

#include "debug.hpp"


void disassemble_chunk(const Chunk& chunk, const std::string name) {
    std::cout << "==" << name << std::string(80, '=') << std::endl;
    std::cout << std::right << std::setw(5) << "CHIDX"
              << "\t"
              << "LINE"
              << "\t" << std::left << std::setw(16) << "OPCODE"
              << "\t" << std::right << std::setw(5) << "CPIDX"
              << "\t" << std::setw(5) << "CVAL" << std::endl;

    for (size_t offset = 0; offset < chunk.opcodes.count( );) {
        offset = disassemble_instruction(chunk, offset);
    }
    std::cout << std::string(100, '=') << std::endl;
}


size_t disassemble_instruction(const Chunk& chunk, size_t offset) {
    std::cout << std::right << std::setw(5) << offset << "\t";
    OpCode instruction = chunk.opcodes[offset];
    uint line = line_number(chunk, offset);
    std::cout << std::setw(5) << line << "\t";
    switch (instruction) {
    case OP_CONSTANT:
        return constant_instruction("OP_CONSTANT", chunk, offset);
    case OP_CONSTANT_LONG:
        return constant_instruction_long("OP_CONSTANT_LONG", chunk, offset);
    case OP_NIL:
        return simple_instruction("OP_NIL", offset);
    case OP_TRUE:
        return simple_instruction("OP_TRUE", offset);
    case OP_FALSE:
        return simple_instruction("OP_FALSE", offset);
    case OP_NOT:
        return simple_instruction("OP_NOT", offset);
    case OP_EQUAL:
        return simple_instruction("OP_EQUAL", offset);
    case OP_NOT_EQUAL:
        return simple_instruction("OP_NOT_EQUAL", offset);
    case OP_GREATER:
        return simple_instruction("OP_GREATER", offset);
    case OP_GREATER_EQUAL:
        return simple_instruction("OP_GREATER_EQUAL", offset);
    case OP_LESS:
        return simple_instruction("OP_LESS", offset);
    case OP_LESS_EQUAL:
        return simple_instruction("OP_LESS_EQUAL", offset);
    case OP_NEGATE:
        return simple_instruction("OP_NEGATE", offset);
    case OP_POP:
        return simple_instruction("OP_POP", offset);
    case OP_POPN:
        return byte_instruction("OP_POPN", chunk, offset);
    case OP_SET_LOCAL:
        return byte_instruction("OP_SET_LOCAL", chunk, offset);
    case OP_SET_LOCAL_LONG:
        return byte_instruction_long("OP_SET_LOCAL_LONG", chunk, offset);
    case OP_GET_LOCAL:
        return byte_instruction("OP_GET_LOCAL", chunk, offset);
    case OP_GET_LOCAL_LONG:
        return byte_instruction_long("OP_GET_LOCAL_LONG", chunk, offset);
    case OP_DEFINE_GLOBAL:
        return constant_instruction("OP_DEFINE_GLOBAL", chunk, offset);
    case OP_DEFINE_GLOBAL_LONG:
        return constant_instruction_long("OP_DEFINE_GLOBAL_LONG", chunk, offset);
    case OP_GET_GLOBAL:
        return constant_instruction("OP_GET_GLOBAL", chunk, offset);
    case OP_GET_GLOBAL_LONG:
        return constant_instruction_long("OP_GET_GLOBAL_LONG", chunk, offset);
    case OP_SET_GLOBAL:
        return constant_instruction("OP_SET_GLOBAL", chunk, offset);
    case OP_SET_GLOBAL_LONG:
        return constant_instruction_long("OP_SET_GLOBAL_LONG", chunk, offset);
    case OP_ADD:
        return simple_instruction("OP_ADD", offset);
    case OP_SUBTRACT:
        return simple_instruction("OP_SUBTRACT", offset);
    case OP_MULTIPLY:
        return simple_instruction("OP_MULTIPLY", offset);
    case OP_DIVIDE:
        return simple_instruction("OP_DIVIDE", offset);
    case OP_PRINT:
        return simple_instruction("OP_PRINT", offset);
    case OP_JUMP:
        return jump_instruction("OP_JUMP", chunk, offset);
    case OP_JUMP_IF_FALSE:
        return jump_instruction("OP_JUMP_IF_FALSE", chunk, offset);
    case OP_RETURN:
        return simple_instruction("OP_RETURN", offset);
    default:
        std::cout << "Unknown opcode " << static_cast<uint>(instruction)
                  << std::endl;
        return offset + 1;
    }
}

size_t simple_instruction(std::string op_name, size_t offset) {
    std::cout << std::setw(16) << std::left << op_name << std::endl;
    return offset + 1;
}

size_t
constant_instruction(std::string op_name, const Chunk& chunk, size_t offset) {
    uint8_t const_idx = chunk.opcodes[offset + 1];
    Value val = chunk.constants[const_idx];
    std::cout << std::setw(16) << std::left << op_name
              << "\t" << std::right << std::setw(5)
              << static_cast<unsigned int>(const_idx) << "\t";
    print_value(val);
    std::cout << std::endl;
    return offset + 2;
}

// NOTE:This method makes assumptions of the byte order and only works on little
// endian machines.
size_t constant_instruction_long(std::string op_name, const Chunk& chunk, size_t offset) {
    uint8_t* b = reinterpret_cast<uint8_t*>(&chunk.opcodes[offset + 1]);
    uint32_t const_idx = (b[0] | b[1] << 8 | b[2] << 16);
    Value val = chunk.constants[const_idx];
    std::cout << std::left << op_name
              << "\t" << std::right << std::setw(5)
              << static_cast<unsigned int>(const_idx) << "\t";
    print_value(val);
    std::cout << std::endl;
    return offset + 4;
}

uint line_number(const Chunk& chunk, size_t offset) {
    for (int i = 0; i < chunk.line_numbers.count( ); i++) {
        if (chunk.line_numbers[i].start <= offset &&
            offset < chunk.line_numbers[i].end) {
            return chunk.line_numbers[i].line;
        }
    }
    return 0;
}
size_t
byte_instruction(std::string op_name, const Chunk& chunk, size_t offset) {
    uint8_t idx = chunk.opcodes[offset + 1];
    std::cout << std::setw(16) << std::left << op_name
              << "\t" << std::right << std::setw(5)
              << static_cast<unsigned int>(idx) << std::endl;
    return offset +2 ;
}

size_t
byte_instruction_long(std::string op_name, const Chunk& chunk, size_t offset) {
    uint8_t* b = reinterpret_cast<uint8_t*>(&chunk.opcodes[offset + 1]);
    uint32_t idx = (b[0] | b[1] << 8 | b[2] << 16);
    std::cout << std::setw(16) << std::left << op_name
              << "\t" << std::right << std::setw(5)
              << static_cast<unsigned int>(idx) << std::endl;
    return offset+4;
}
size_t
jump_instruction(std::string op_name, const Chunk& chunk, size_t offset) {
    uint16_t jump = twobyte_idx(&chunk.opcodes[offset+1]);
    std::cout << std::setw(16) << std::left << op_name
              << "\t" << std::right << std::setw(5)
              << static_cast<unsigned int>(jump) << std::endl;
    return offset+3;

}