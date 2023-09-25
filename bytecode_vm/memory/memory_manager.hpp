//
// Created by Johan Ericsson on 2023-08-30.
//

#ifndef CLOXPP_MEMORY_MANAGER_HPP
#define CLOXPP_MEMORY_MANAGER_HPP

#include "allocators.hpp"

namespace memory {

template <typename Allocator>
class MemoryManager : private Allocator {
    void collect();

    public:
    void* allocate(size_t size) {
        return Allocator::allocate(size);
    }
    void deallocate(void* ptr) {
        Allocator::deallocate(ptr);
    }
};

} // namespace memory


#endif    // CLOXPP_MEMORY_MANAGER_HPP
