//
// Created by Johan Ericsson on 2023-05-16.
//

#ifndef CLOXPP_MEMORY_POOL_HPP
#define CLOXPP_MEMORY_POOL_HPP

#include <cstddef>
template <std::size_t chunk_size, std::size_t chunks_per_block>
class MemoryPool {
public:
    //static_assert(chunk_size >=);
    static_assert(chunks_per_block>0);

    explicit MemoryPool() = default;

    MemoryPool(const MemoryPool&) = delete;

    MemoryPool(MemoryPool&& mem_pool) {}

    ~MemoryPool() {}

    MemoryPool& operator=(MemoryPool  mem_pool) {
        return *this;
    }

    void* allocate();
    void deallocate(void* ptr);

};



#endif //CLOXPP_MEMORY_POOL_HPP
