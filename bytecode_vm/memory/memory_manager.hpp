//
// Created by Johan Ericsson on 2023-08-30.
//

#ifndef CLOXPP_MEMORY_MANAGER_HPP
#define CLOXPP_MEMORY_MANAGER_HPP

#include "allocators.hpp"
#include "object.hpp"

namespace memory {
class MemoryManager {};
    PoolAllocator<Mallocator, sizeof(StringObject), 256> str_obj_pool();

} // namespace memory

#endif    // CLOXPP_MEMORY_MANAGER_HPP
