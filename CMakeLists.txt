cmake_minimum_required(VERSION 3.20)
project(cloxpp)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCE_FILES_TREEWALK treewalk_interpreter/main.cpp treewalk_interpreter/main.cpp treewalk_interpreter/token.cpp treewalk_interpreter/token.hpp treewalk_interpreter/scanner.cpp treewalk_interpreter/scanner.hpp treewalk_interpreter/error_handling.cpp treewalk_interpreter/error_handling.hpp)

set(SOURCE_FILES_VM bytecode_vm/main.cpp bytecode_vm/chunk.cpp bytecode_vm/chunk.hpp bytecode_vm/dynamic_array.hpp bytecode_vm/memory.hpp bytecode_vm/memory.cpp bytecode_vm/debug.cpp bytecode_vm/debug.hpp bytecode_vm/value.hpp bytecode_vm/value.cpp bytecode_vm/virtual_machine.hpp bytecode_vm/virtual_machine.cpp bytecode_vm/stack.hpp bytecode_vm/stack.cpp bytecode_vm/binary_operators.hpp bytecode_vm/compiler.cpp bytecode_vm/compiler.hpp bytecode_vm/scanner.cpp bytecode_vm/scanner.hpp bytecode_vm/token.cpp bytecode_vm/token.hpp bytecode_vm/parser.hpp bytecode_vm/parser.cpp)


# Add executable target with source files listed in SOURCE_FILES variable
add_executable(cloxsimple ${SOURCE_FILES_TREEWALK})
add_executable(cloxpp ${SOURCE_FILES_VM})
