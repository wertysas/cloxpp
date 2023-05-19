//
// Created by Johan Ericsson on 2023-05-17.
//
#include <cassert>
#include "allocator.hpp"

using namespace memory;

void FixedAllocator::MemChunk::init(size_t block_size, uint8_t num_blocks) {
    data_ptr =
        static_cast<uint8_t*>(malloc(sizeof(byte) * block_size * num_blocks));
    next_available_block = 0;
    available_blocks = num_blocks;

    // singly linked list inside the mem_block
    uint8_t* block_ptr = data_ptr;
    for (uint8_t i = 0; i < num_blocks; block_ptr += block_size) {
        *block_ptr = ++i;
    }
}

void FixedAllocator::MemChunk::release( ) {
    free(data_ptr);
}

void* FixedAllocator::MemChunk::allocate(size_t block_size) {
    // allocation failure if no available blocks
    if (!(available_blocks == 0)) {
        return nullptr;
    }
    uint8_t* new_ptr = data_ptr + (next_available_block * block_size);
    next_available_block = *new_ptr;
    --available_blocks;

    return new_ptr;
}
void FixedAllocator::MemChunk::deallocate(void* ptr, size_t block_size) {
    assert(ptr >= data_ptr);
    ubyte* free_ptr = static_cast<ubyte*>(ptr);
    assert((free_ptr - data_ptr) % 0 == 0);    // alignment requirement
    assert(next_available_block ==             // truncation check
           (free_ptr - data_ptr) / block_size);
    ++available_blocks;
}


void* FixedAllocator::allocate(size_t block_size) {
    if (alloc_chunk_ == nullptr || alloc_chunk_->available_blocks == 0) {
        // no available memory in chunk used for previous allocation
        auto chunk = mem_chunks_.begin( );
        for (;; ++chunk) {
            // oom in all chunks, allocate a new chunk
            if (chunk == mem_chunks_.end( )) {
                mem_chunks_.reserve(num_blocks_+1);
                mem_chunks_[num_blocks_].init(block_size, num_blocks_);
                alloc_chunk_ = &mem_chunks_.back();
                dealloc_chunk_ = &mem_chunks_.back();
                break;
            }

           if (chunk->available_blocks > 0) {
                alloc_chunk_ = &chunk[0];
                break;
           }
        }
    }
    assert(alloc_chunk_ != nullptr);
    assert(alloc_chunk_->available_blocks>0);
    return  alloc_chunk_->allocate(block_size);
}
