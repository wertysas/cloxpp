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
    public:
    void* allocate(size_t size) {
#ifdef DEBUG_STRESS_GC
        collector_.collect();
#endif
        return Allocator::allocate(size);
    }
    void deallocate(void* ptr) {
        Allocator::deallocate(ptr);
    }
    void init(VirtualMachine* vm) {
        collector_.init(vm);
    }
};


} // namespace memory


#endif    // CLOXPP_MEMORY_MANAGER_HPP
