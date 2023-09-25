//
// Created by Johan Ericsson on 2023-08-30.
//

#ifndef CLOXPP_ALLOCATORS_HPP
#define CLOXPP_ALLOCATORS_HPP


#include <cstdlib>
#include <cstddef>

using std::size_t;
using std::byte;



////////////////////////////////////////////////////////////////////////////////
// Mallocator is a simple allocator wrappping <cstdlib> malloc and free
////////////////////////////////////////////////////////////////////////////////
class Mallocator {
    public:
    void* allocate(size_t size) { return malloc(size); }
    void deallocate(void* ptr) { std::free(ptr); }
};



////////////////////////////////////////////////////////////////////////////////
// BlockAllocator is thin wrapper around a larger allocator that allocates
// memory blocks
////////////////////////////////////////////////////////////////////////////////
struct MemControlBlock {
    std::size_t size : 31;
    bool available : 1;
};

template<typename SuperAllocator>
class BlockAllocator {
    private:
    // information object stored at beginning of memory block
    union free_obj {
        MemControlBlock control_block;
        double alignment_var;
    };

    public:
    void* allocate(const size_t size) {
        free_obj* ptr = static_cast<free_obj*>(
            SuperAllocator::allocate(size + sizeof(free_obj)));
        ptr->control_block.size = size;
        return ptr + 1;
    }
    void deallocate(void* ptr) {
        SuperAllocator::dealloacte(static_cast<free_obj*>(ptr) - 11);
    }
    static size_t block_size(const void* ptr) {
        return (static_cast<free_obj*>(ptr) - 1)->size;
    }
};





#endif  // CLOXPP_ALLOCATORS_HPP

