//
// Created by Johan Ericsson on 2023-04-04.
//

#ifndef CLOXPP_DEBUG_HPP
#define CLOXPP_DEBUG_HPP

#include <string>
#include "chunk.hpp"

void disassemble_chunk(Chunk& chunk, std::string name);
size_t disassemble_instruction(Chunk& chunk, size_t offset);
size_t simple_instruction(std::string str, size_t offset);


#endif //CLOXPP_DEBUG_HPP
