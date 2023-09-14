cmake_minimum_required(VERSION 3.20)
project(cloxpp)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")


add_subdirectory(tests)

# cloxpp is the bytecode intepreter
include_directories(bytecode_vm)
set(SOURCE_FILES_VM
        bytecode_vm/main.cpp
        bytecode_vm/chunk.cpp
        bytecode_vm/memory/memory.cpp
        bytecode_vm/debug.cpp
        bytecode_vm/value.cpp
        bytecode_vm/virtual_machine.cpp
        bytecode_vm/stack.cpp
        bytecode_vm/compiler.cpp
        bytecode_vm/scanner.cpp
        bytecode_vm/token.cpp
        bytecode_vm/parser.cpp
        bytecode_vm/error_handling.cpp
        bytecode_vm/object.cpp
        bytecode_vm/cpp_module.cpp
        bytecode_vm/memory/mark_sweep_collector.cpp
        bytecode_vm/memory/mark_sweep_collector.hpp
        bytecode_vm/memory/memory_manager.cpp
        bytecode_vm/memory/memory_manager.hpp
        bytecode_vm/memory/allocators.hpp)

add_executable(cloxpp ${SOURCE_FILES_VM})