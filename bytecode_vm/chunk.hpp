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

// Struct holding lineinfo for a line including start and end
// offset of instructions in chunk if start=end, then empty
struct LineInfo {
    uint line;
    uint start;
    uint end;
    LineInfo() : line(0), start(0), end(0) {};
    LineInfo(uint line_, uint start_) : line(line_), start(start_), end(start_+1) {};
};

class Chunk {
    public:
        DynamicArray<OpCode> opcodes;
        DynamicArray<Value> constants;
        DynamicArray<LineInfo> line_numbers;
        void add_opcode(OpCode opcode, uint line);
        void add_constant(Value constant, uint line);
        Chunk();
};


inline uint32_t constant_long_idx(OpCode* op) {
    uint8_t* b = reinterpret_cast<uint8_t*>(op);
    uint32_t const_idx = (b[0] | b[1] << 8 | b[2] << 16);
    return const_idx;
}

#endif //CLOXPP_CHUNK_HPP
