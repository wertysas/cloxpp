//
// Created by Johan Ericsson on 2023-08-30.
//

#ifndef CLOXPP_MEMORY_MANAGER_HPP
#define CLOXPP_MEMORY_MANAGER_HPP

#include "common.hpp"
#include "allocators.hpp"
#include "mark_sweep_collector.hpp"



namespace memory {

template <typename Allocator>
class MemoryManager : private Allocator {

    MarkSweepCollector collector_;
    size_t allocated_size=0; // bytes
    size_t gc_threshold =1024*1024;
    constexpr int heap_growth_factor() { return 2; }
    inline void check_size() {
        if (allocated_size > gc_threshold) {
            collect();
        }
    }
    public:
    void* allocate(size_t size) {
        // TODO: add checks for successful allocation
        allocated_size += size;
#ifdef DEBUG_STRESS_GC
        collect();
#else
        check_size( );
#endif
        return Allocator::allocate(size);
    }

    void* reallocate(void* ptr, size_t old_size, size_t new_size) {
        allocated_size += new_size-old_size;

#ifdef DEBUG_STRESS_GC
        if (new_size > old_size) {
            collect();
        }
#else
        check_size();
#endif
        if (new_size == 0) {
            free(ptr);
            return nullptr;
        }
        void* res;
        res = realloc(ptr, new_size);
        if (res == nullptr) {
            exit(1);
        }
        return res;
    }

    void deallocate(void* ptr, size_t size) {
        allocated_size -= size;
        Allocator::deallocate(ptr);
    }
    void set(VirtualMachine* vm) { collector_.set(vm);
    }
    void set(Parser* parser) { collector_.set(parser);
    }

    void* allocate_array(size_t type_size, size_t count) {
        size_t size = type_size*count;
        allocated_size += size;
#ifdef DEBUG_STRESS_GC
        collect();
#else
        check_size();
#endif
        return Allocator::allocate(size);
    }
    void collect() {
        collector_.collect();
        gc_threshold = allocated_size*heap_growth_factor();
    }
};


} // namespace memory


#endif    // CLOXPP_MEMORY_MANAGER_HPP
