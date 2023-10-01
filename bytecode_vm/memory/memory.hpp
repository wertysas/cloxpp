//
// Created by Johan Ericsson on 2023-04-03.
//

#ifndef CLOXPP_MEMORY_HPP
#define CLOXPP_MEMORY_HPP

#include <iostream>
#include <stack>
#include "common.hpp"
#include "memory_manager.hpp"
#include "allocators.hpp"
#include "object_fwd.hpp"

#define INIT_CAPACITY 8
#define GROWTH_CONSTANT 2


void free_objects( );
void free_object(Object* object);


namespace memory {

extern Object* objects;
extern MemoryManager<Mallocator> memory_manager;
extern std::stack<Object*> grey_list;

template<typename T>
T* allocate_array(size_t count) {
    void* ptr = memory_manager.allocate_array(sizeof(T), count);
#ifdef DEBUG_LOG_GC
                    std::cout
                << ptr << " allocated array\tsize: " << sizeof(T) * count
                << "\tcount: " << count << "\ttype: " << typeid(T).name( ) << "\n";
#endif
    return reinterpret_cast<T*>(ptr);
}


template<typename T>
void free(void* ptr) {
#ifdef DEBUG_LOG_GC
    std::cout << ptr << " free object of type " << typeid(T).name( ) << "\n";
#endif
    memory_manager.deallocate(ptr, sizeof(T));
}

template<typename T>
void free_array(void* ptr, size_t count) {
#ifdef DEBUG_LOG_GC
    std::cout << ptr << " free array of type " << typeid(T).name( )
              << " and count: " << count << "\n";
#endif
    memory_manager.deallocate(ptr, sizeof(T) * count);
}

void mark_object(Object* obj);
void mark_black(Object* obj);


}    // end of namespace memory


#endif    // CLOXPP_MEMORY_HPP
