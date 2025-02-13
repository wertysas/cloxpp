//
// Created by Johan Ericsson on 2023-04-03.
//

#ifndef CLOXPP_CHUNK_HPP
#define CLOXPP_CHUNK_HPP

#include "common.hpp"
#include "value.hpp"
#include "dynamic_array.hpp"


// If one runs out of instructions, an idea could be to encode long instructions
// by an extra opcode LONG, and if the VM reaches a "LONG" instruction it knows
// it has to interpret the next opcode + xx bytes differently. This way the
// slower path only has to be taken if reaching the "LONG" instruction

enum OpCode : uint8_t {
    // Constants
    OP_CONSTANT,
    OP_CONSTANT_LONG,
    // Variables
    OP_SET_LOCAL,
    OP_SET_LOCAL_LONG,
    OP_GET_LOCAL,
    OP_GET_LOCAL_LONG,
    OP_DEFINE_GLOBAL,
    OP_DEFINE_GLOBAL_LONG,
    OP_SET_GLOBAL,
    OP_SET_GLOBAL_LONG,
    OP_GET_GLOBAL,
    OP_GET_GLOBAL_LONG,
    OP_SET_UPVALUE,
    OP_SET_UPVALUE_LONG,
    OP_GET_UPVALUE,
    OP_GET_UPVALUE_LONG,
    OP_SET_PROPERTY,
    OP_SET_PROPERTY_LONG,
    OP_GET_PROPERTY,
    OP_GET_PROPERTY_LONG,
    OP_GET_SUPER,
    OP_INVOKE,
    OP_INVOKE_LONG,
    OP_SUPER_INVOKE,

    // Null, Boolean and Comparisons
    OP_NIL,
    OP_TRUE,
    OP_FALSE,
    OP_NOT,
    OP_EQUAL,
    OP_NOT_EQUAL,
    OP_GREATER,
    OP_GREATER_EQUAL,
    OP_LESS,
    OP_LESS_EQUAL,
    OP_NEGATE,
    // Stack Manipulation
    OP_POP,
    OP_POPN,
    // Binary Operators
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    // I/O
    OP_PRINT,
    // CONTROL FLOW
    OP_JUMP,
    OP_JUMP_IF_FALSE,
    OP_JUMP_IF_TRUE,
    OP_LOOP,
    OP_CALL,
    OP_CLOSURE,
    OP_CLOSURE_LONG,
    OP_CLOSE_UPVALUE,
    OP_CLASS,
    OP_CLASS_LONG,
    OP_INHERIT,
    OP_METHOD,
    OP_RETURN,
};


// Struct holding lineinfo for a line including start and end
// offset of instructions in chunk if start=end, then empty
struct LineInfo {
    uint line; // line of tokens parsed to opcode
    uint start; // first opcode corresponding to line
    uint end; // last opcode added to chunk
    LineInfo() : line(0), start(0), end(0) {};
    LineInfo(uint line_, uint start_) : line(line_), start(start_), end(start_) {};
};


class Chunk {
    private:
    static Value* tmp_value; // GC tmp for tracking values before resizing

    public:
        // constants must be first, initialization order is important for GC
        DynamicArray<Value> constants;
        DynamicArray<OpCode> opcodes;
        DynamicArray<LineInfo> line_numbers;
        void add_opcode(OpCode opcode, uint line);
        uint add_constant(Value constant);
        uint add_constant(Value constant, uint line);
        Chunk();
        // mark_value_array
        void mark_constants();
};


inline uint32_t constant_long_idx(OpCode* op) {
    uint8_t* b = reinterpret_cast<uint8_t*>(op);
    uint32_t const_idx = (b[0] | b[1] << 8 | b[2] << 16);
    return const_idx;
}

inline uint16_t twobyte_idx(OpCode* op) {
    uint8_t* b = reinterpret_cast<uint8_t*>(op);
    uint16_t twobyte_idx = (b[0] | b[1] << 8);
    return twobyte_idx;
}
#endif //CLOXPP_CHUNK_HPP
