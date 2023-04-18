//
// Created by Johan Ericsson on 2023-04-03.
//

#ifndef CLOXPP_CHUNK_HPP
#define CLOXPP_CHUNK_HPP

#include "common.hpp"
#include "value.hpp"
#include "dynamic_array.hpp"


enum OpCode : uint8_t {
    OP_RETURN,
    OP_CONSTANT,
    OP_CONSTANT_LONG,
};


struct LineInfo {
    uint count;
    uint line;
    LineInfo() : count(0), line(0) {};
    LineInfo(uint count_, uint line_) : count(count_), line(line_) {};
};

class Chunk {
    public:
        DynamicArray<OpCode> opcodes;
        DynamicArray<Value> constants;
        DynamicArray<LineInfo> line_numbers;
        void add_opcode(OpCode opcode, uint line);
        void add_constant(Value constant);
        Chunk();
};

// size_t size_from_bytes(uint8_t* b) {
//     size_t size = (b[0] << 16 | b[1] << 8 | b[2]);
//     return size;
// }

//using Chunk = DynamicArray<OpCode>;

#endif //CLOXPP_CHUNK_HPP
