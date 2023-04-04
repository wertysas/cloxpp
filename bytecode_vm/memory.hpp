//
// Created by Johan Ericsson on 2023-04-03.
//

#ifndef CLOXPP_MEMORY_HPP
#define CLOXPP_MEMORY_HPP

#include <iostream>
#include "common.hpp"


#define INIT_CAPACITY 8
#define GROWTH_CONSTANT 2

void* reallocate(void* ptr, size_t old_size, size_t new_size);


#endif //CLOXPP_MEMORY_HPP
