//
// Created by Johan Ericsson on 2023-04-03.
//


#include "common.hpp"
#include "dynamic_array.hpp"
#include "chunk.hpp"
#include "debug.hpp"

int main(int argc, char* argv[]) {
    Chunk chunk{};
    double x = 1.5, y;
    for (int i=1; i< 300; i++) {
        y = x*i;
        chunk.add_constant(y, 123);
    }
    chunk.add_opcode(OP_RETURN, 124);
    disassemble_chunk(chunk, "test chunk");
    return 0;
}
