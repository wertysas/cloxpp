//
// Created by Johan Ericsson on 7/8/23.
//

#include "cpp_module.hpp"
#include <iostream>
#include <ctime>

Value native_clock(uint arg_count, Value* args) {
    std::cout << "Entered native clock" << std::endl;
    return Value(static_cast<double>(clock())/CLOCKS_PER_SEC);
}
