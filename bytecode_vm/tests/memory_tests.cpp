//
// Created by Johan Ericsson on 2023-05-17.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "../memory/memory_pool.hpp" //FIXME: WHY IS IT NOT FINDING WHY THIS HEADER!?


TEST_CASE("memory pool") {
    MemoryPool<sizeof(int), 3> mem_pool;
}