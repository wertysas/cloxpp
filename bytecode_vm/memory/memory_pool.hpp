//
// Created by Johan Ericsson on 2023-05-16.
//

#ifndef CLOXPP_MEMORY_POOL_HPP
#define CLOXPP_MEMORY_POOL_HPP

#include <cstddef>
#include <list>
#include <utility>

#define DEBUG
#ifdef DEBUG
#include <iostream>
#include <iomanip>
#endif

using std::size_t;
using std::cout;
using std::endl;


template<size_t chunk_size, size_t chunks_per_block>
class MemoryPool {
    public:
    static_assert(chunk_size > 0);
    static_assert(chunks_per_block > 0);

    explicit MemoryPool( ) = default;

    MemoryPool(const MemoryPool&) = delete;

    MemoryPool(MemoryPool&& mem_pool) {}

    ~MemoryPool( ) {}

    MemoryPool& operator=(MemoryPool mem_pool) { return *this; }

    void* allocate( ) {}

    void deallocate(void* ptr) {}

    private:
    struct mem_chunk {
        mem_chunk(mem_chunk* next = nullptr) : mem_next(next) {
            mem_sequence = ++mem_seq_count;
#ifdef DEBUG
            std::cout << "constructed chunk # " << std::setw(2)
                      << std::setfill('0') << mem_sequence << " " << this
                      << " -> " << mem_next << endl;
#endif
        }
        union {
            mem_chunk* mem_next = nullptr;
            std::byte mem_storage[chunk_size];
        };
        size_t mem_sequence;
        inline static size_t mem_seq_count;
    };
    std::list<void*> mem_blocks;
};


#endif    // CLOXPP_MEMORY_POOL_HPP
