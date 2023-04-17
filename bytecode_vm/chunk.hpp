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


class Chunk {
    public:
        DynamicArray<OpCode> opcodes;
        DynamicArray<Value> constants;
        void add_opcode(OpCode opcode);
        void add_constant(Value constant);
};

// size_t size_from_bytes(uint8_t* b) {
//     size_t size = (b[0] << 16 | b[1] << 8 | b[2]);
//     return size;
// }

//using Chunk = DynamicArray<OpCode>;

#endif //CLOXPP_CHUNK_HPP
