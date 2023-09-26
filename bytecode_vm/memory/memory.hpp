//
// Created by Johan Ericsson on 2023-04-03.
//

#ifndef CLOXPP_MEMORY_HPP
#define CLOXPP_MEMORY_HPP

#include <iostream>
#include "common.hpp"
#include "memory_manager.hpp"
#include "allocators.hpp"

#define INIT_CAPACITY 8
#define GROWTH_CONSTANT 2


void* reallocate(void* ptr, size_t old_size, size_t new_size);

struct Object;
void free_objects( );
void free_object(Object* object);


namespace memory {

extern Object* objects;
extern MemoryManager<Mallocator> memory_manager;

template<typename T>
T* allocate(uint count) {
    return reinterpret_cast<T*>(memory_manager.allocate(sizeof(T) * count));
}

template<typename T>
void free(void* ptr) {
#ifdef DEBUG_LOG_GC
    std::cout << ptr << " free object of type " << typeid(T).name( ) << "\n";
#endif
    memory_manager.deallocate(ptr);
}

template<typename T>
void free_array(void* ptr, size_t size) {
    reallocate(ptr, sizeof(T) * size, 0);
}


}    // end of namespace memory


#endif    // CLOXPP_MEMORY_HPP
