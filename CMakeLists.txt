cmake_minimum_required(VERSION 3.20)
project(cloxpp)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(tests)

# cloxpp is the bytecode intepreter
set(SOURCE_FILES_VM
        bytecode_vm/main.cpp
        bytecode_vm/chunk.cpp
        bytecode_vm/chunk.hpp
        bytecode_vm/dynamic_array.hpp
        bytecode_vm/static_array.hpp
        bytecode_vm/memory.hpp
        bytecode_vm/memory.cpp
        bytecode_vm/debug.cpp
        bytecode_vm/debug.hpp
        bytecode_vm/value.hpp
        bytecode_vm/value.cpp
        bytecode_vm/virtual_machine.hpp
        bytecode_vm/virtual_machine.cpp
        bytecode_vm/stack.hpp
        bytecode_vm/stack.cpp
        bytecode_vm/binary_operators.hpp
        bytecode_vm/compiler.cpp
        bytecode_vm/compiler.hpp
        bytecode_vm/scanner.cpp
        bytecode_vm/scanner.hpp
        bytecode_vm/token.cpp
        bytecode_vm/token.hpp
        bytecode_vm/parser.hpp
        bytecode_vm/parser.cpp
        bytecode_vm/error_handling.hpp
        bytecode_vm/error_handling.cpp
        bytecode_vm/object.cpp bytecode_vm/object.hpp bytecode_vm/hash_table.hpp)

add_executable(cloxpp ${SOURCE_FILES_VM})
