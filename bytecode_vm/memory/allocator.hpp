//
// Created by Johan Ericsson on 2023-05-16.
// Allocator based on the small object allocator featured in Ch. 4 of modern C++
// design by Andrei Alexandrescu. For the source code to the book see:
// https://sourceforge.net/projects/loki-lib/files/Loki/
//

#ifndef CLOXPP_ALLOCATOR_HPP
#define CLOXPP_ALLOCATOR_HPP

#include "mem_common.hpp"

namespace memory {


//  "...for efficiency reasons, Chunk does not define constructors, destructor,
//  or assignment operator. Defining proper copy semantics at this level hurts
//  efficiency at upper levels, where we store Chunk objects in a vector."
//  quoted from Modern C++ Design


class FixedAllocator {
    private:
    struct MemChunk {
        uint8_t* data_ptr;
        uint8_t available_blocks;
        uint8_t next_available_block;

        void init(size_t block_size,
                  uint8_t num_blocks);    // Maximum of 256 blocks
        void release( );
        void* allocate(size_t block_size);
        void deallocate(void* ptr, size_t block_size);

        // to assure that ctors and assignment operators are deleted
        MemChunk( ) = delete;
        MemChunk(const MemChunk&) = delete;
        MemChunk& operator=(const MemChunk&) = delete;


    };
    size_t block_size_;
    ubyte num_blocks_;
    std::vector<MemChunk> mem_chunks_;
    MemChunk* alloc_chunk_; // MemChunk for last allocation
    MemChunk* dealloc_chunk_; // MemChunk for last deallocation

    public:
    void* allocate(size_t block_size);
    void deallocate(void* ptr, size_t block_size);
};


class SmallObjectAllocator {
    public:
    SmallObjectAllocator(size_t chunk_size, size_t max_object_size);
    void* allocate(size_t bytes);
    void deallocate(void* ptr, size_t size);

    private:
    std::vector<FixedAllocator> alloc_pool_;
    FixedAllocator* prev_alloc_;
    FixedAllocator* prev_dealloc_;

};


}    // namespace memory
#endif    // CLOXPP_ALLOCATOR_HPP
