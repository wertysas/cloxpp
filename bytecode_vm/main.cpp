//
// Created by Johan Ericsson on 2023-04-03.
//


#include "common.hpp"
#include "dynamic_array.hpp"
#include "chunk.hpp"
#include "debug.hpp"
#include "virtual_machine.hpp"

int main(int argc, char* argv[]) {
    Chunk chunk{};

    chunk.add_constant(1.2, 123);
    chunk.add_constant(3.4, 123);
    chunk.add_opcode(OP_ADD, 123);
    chunk.add_constant(5.6, 123);
    chunk.add_opcode(OP_DIVIDE, 123);
    chunk.add_opcode(OP_NEGATE,123);
    chunk.add_opcode(OP_RETURN,123);


    VirtualMachine VM{};
    disassemble_chunk(chunk, "test chunk");
    VM.interpret(&chunk);
    return 0;
}
