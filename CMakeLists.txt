cmake_minimum_required(VERSION 3.20)  # CMake version check
project(cloxpp)               # Create project "simple_example"
set(CMAKE_CXX_STANDARD 20)            # Enable c++14 standard

# Add main.cpp file of project root directory as source file
set(SOURCE_FILES main.cpp main.cpp token.cpp token.hpp scanner.cpp scanner.hpp error_handling.cpp error_handling.hpp)

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(cloxpp ${SOURCE_FILES})