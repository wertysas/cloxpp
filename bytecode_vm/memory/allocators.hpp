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
template<typename SuperAllocator>
class BlockAllocator {
    private:
    // information object stored at beginning of memory block
    union free_obj {
        size_t size;
        double alignment_var;
    };

    public:
    void* allocate(const size_t size) {
        free_obj* ptr = static_cast<free_obj*>(
            SuperAllocator::allocate(size + sizeof(free_obj)));
        ptr->size = size;
        return ptr + 1;
    }
    void deallocate(void* ptr) {
        SuperAllocator::dealloacte(static_cast<free_obj*>(ptr) - 11);
    }
    static size_t block_size(const void* ptr) {
        return (static_cast<free_obj*>(ptr) - 1)->size;
    }
};



////////////////////////////////////////////////////////////////////////////////
// PoolAllocator is memory pool allocator which allocates fixed size chunks
////////////////////////////////////////////////////////////////////////////////

template<typename SuperAllocator, size_t ChunkSize, size_t Chunks>
class PoolAllocator : public SuperAllocator {
    static_assert(ChunkSize > sizeof(size_t));
    constexpr size_t PoolSize( ) { return ChunkSize * Chunks; }

    public:
    PoolAllocator( ) : SuperAllocator(), free_list_(nullptr) {
        mem_pool_ = static_cast<byte*>(SuperAllocator::allocate(PoolSize( )));
        for (size_t j = Chunks; j > 0; --j) {
            size_t i = j-1;
            Chunk* chunk = reinterpret_cast<Chunk*>(&mem_pool_[i * ChunkSize]);
            chunk->next = free_list_;
            free_list_ = chunk;
        }
#ifdef DEBUG_PRINT
        std::cout << "Constructed PoolAllocator with ChunkSize: "
                  << ChunkSize << " Chunks: " << Chunks << std::endl;
#endif
    }
    ~PoolAllocator( ) { SuperAllocator::deallocate(mem_pool_); }

    void* allocate( ) {
        Chunk* chunk = free_list_;
        if (chunk != nullptr) {
            free_list_ = free_list_->next;
        }
        return chunk;
    }
    void deallocate(void* ptr) {
        if (ptr < mem_pool_ || ptr >= mem_pool_ + PoolSize( )) {
            return;
        }
        Chunk* chunk = static_cast<Chunk*>(ptr);
        chunk->next = free_list_;
        free_list_ = chunk;
    }

    private:
    struct Chunk {
        union {
            Chunk* next;
            byte data[ChunkSize];
        };
    };
    byte* mem_pool_;      // Pointer to start of memory pool
    Chunk* free_list_;    // Linked list of free Chunks in pool
};


////////////////////////////////////////////////////////////////////////////////
// ObjectAllocator is a memory pool allocator which allocates objects of type T
////////////////////////////////////////////////////////////////////////////////

template<typename T,
    size_t Size,
    typename SuperAllocator = PoolAllocator<Mallocator, sizeof(T), Size>>
class ObjectPool : public SuperAllocator {
    public:
    ObjectPool( ) : SuperAllocator( ) {
#ifdef DEBUG_PRINT
        std::cout << "Constructed ObjectPool for type: " << typeid(T).name( )
                  << std::endl;
#endif
    };
    ~ObjectPool( ) { SuperAllocator::~SuperAllocator( ); };
    void* allocate( ) {
        void* ptr = SuperAllocator::allocate( );
        T* t = new (ptr) T(); // In place new
        return t;
    }
    void deallocate(T* ptr) {
        ptr->~T( );
        PoolAllocator<Mallocator, sizeof(T), Size>::deallocate(ptr);
    }
};

#endif    // CPPEXAMPLES_ALLOCATORS_HPP

