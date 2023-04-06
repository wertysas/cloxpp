//
// Created by Johan Ericsson on 2023-04-03.
//


#include "common.hpp"
#include "dynamic_array.hpp"
#include "chunk.hpp"
#include "debug.hpp"

int main(int argc, char* argv[]) {
    Chunk chunk{};
    chunk.opcodes.append(OP_RETURN);
    disassemble_chunk(chunk, "test chunk");
    return 0;
}
