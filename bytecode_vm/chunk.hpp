//
// Created by Johan Ericsson on 2023-04-03.
//

#ifndef CLOXPP_CHUNK_HPP
#define CLOXPP_CHUNK_HPP

#include "common.hpp"
#include "dynamic_array.hpp"


enum OpCode : uint8_t {
    OP_RETURN
};

using Chunk = DynamicArray<OpCode>;

#endif //CLOXPP_CHUNK_HPP
