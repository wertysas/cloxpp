//
// Created by Johan Ericsson on 2023-04-03.
//


#include "common.hpp"
#include "dynamic_array.hpp"
#include "chunk.hpp"
#include "debug.hpp"

int main(int argc, char* argv[]) {
    Chunk chunk{};
    chunk.add_opcode(OP_RETURN, 123);
    double x = 1.0, y;
    for (int i=1; i< 300; i++) {
        y = x*i;
        chunk.add_constant(y, 123);
    }
    for (uint i=0; i<chunk.line_numbers.count(); i++) {
        std::cout << "LINE: " << chunk.line_numbers[i].line
        << "\tSTART: " << chunk.line_numbers[i].start
        << "\tEND: " << chunk.line_numbers[i].end << std::endl;
    }
    disassemble_chunk(chunk, "test chunk");
    return 0;
}
