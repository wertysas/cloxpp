cmake_minimum_required(VERSION 3.20)
project(cloxpp)
set(CMAKE_CXX_STANDARD 20)


# Add main.cpp file of project root directory as source file
set(SOURCE_FILES_TREEWALK treewalk_interpreter/main.cpp treewalk_interpreter/main.cpp treewalk_interpreter/token.cpp treewalk_interpreter/token.hpp treewalk_interpreter/scanner.cpp treewalk_interpreter/scanner.hpp treewalk_interpreter/error_handling.cpp treewalk_interpreter/error_handling.hpp)

set(SOURCE_FILES_VM bytecode_vm/main.cpp bytecode_vm/chunk.cpp bytecode_vm/chunk.hpp bytecode_vm/dynamic_array.hpp bytecode_vm/memory.hpp bytecode_vm/memory.cpp bytecode_vm/debug.cpp bytecode_vm/debug.hpp bytecode_vm/value.hpp bytecode_vm/value.cpp)


# Add executable target with source files listed in SOURCE_FILES variable
add_executable(cloxsimple ${SOURCE_FILES_TREEWALK})
add_executable(cloxpp ${SOURCE_FILES_VM})
