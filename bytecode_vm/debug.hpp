//
// Created by Johan Ericsson on 2023-04-04.
//

#ifndef CLOXPP_DEBUG_HPP
#define CLOXPP_DEBUG_HPP

#include <string>
#include "chunk.hpp"

void disassemble_chunk(const Chunk& chunk, std::string name);

size_t disassemble_instruction(const Chunk& chunk, size_t offset);

size_t simple_instruction(std::string op_name, size_t offset);

size_t
constant_instruction(std::string op_name, const Chunk& chunk, size_t offset);
size_t constant_instruction_long(std::string op_name,
                                 const Chunk& chunk,
                                 size_t offset);

size_t byte_instruction(std::string op_name, const Chunk& chunk, size_t offset);
size_t
byte_instruction_long(std::string op_name, const Chunk& chunk, size_t offset);

uint line_number(const Chunk& chunk, size_t offset);

#endif    // CLOXPP_DEBUG_HPP
