//
// Created by Johan Ericsson on 2023-04-03.
//

#ifndef CLOXPP_COMMON_HPP
#define CLOXPP_COMMON_HPP

#include <stdint.h> // typedefs for fixed width integer types and macro constants
#include <cstddef>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

using uint = unsigned int;
using std::string;
using std::cin;
using std::cout;


#define FRAMES_MAX 64 // maximum call frame size
#define STACK_MAX (FRAMES_MAX*(UINT8_MAX+1)) // default stack size

#define DEBUG_PRINT_CODE
#define DEBUG_TRACE_EXECUTION

#endif //CLOXPP_COMMON_HPP
