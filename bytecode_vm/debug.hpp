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

size_t invoke_instruction(const char* op_name,
                          const Chunk& chunk,
                          size_t offset,
                          bool long_instruction=false);

    size_t byte_instruction(std::string op_name, const Chunk& chunk, size_t offset);
size_t
byte_instruction_long(std::string op_name, const Chunk& chunk, size_t offset);

size_t jump_instruction(std::string op_name, const Chunk& chunk, size_t offset, int sign=1);

size_t closure_instruction(std::string op_name,
                           const Chunk& chunk,
                           size_t offset,
                           uint const_idx,
                           uint line);

uint line_number(const Chunk& chunk, size_t offset);

#endif    // CLOXPP_DEBUG_HPP
