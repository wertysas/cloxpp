//
// Created by Johan Ericsson on 2023-04-03.
//

#ifndef CLOXPP_MEMORY_HPP
#define CLOXPP_MEMORY_HPP

#include <iostream>
#include <stack>
#include <list>
#include "common.hpp"
#include "memory_manager.hpp"
#include "allocators.hpp"
#include "object_fwd.hpp"
#include "hash_table.hpp"

#define INIT_CAPACITY 8
#define GROWTH_CONSTANT 2


void free_objects( );
void free_object(Object* object);


namespace memory {

extern Object* objects;
extern std::list<Object*> temporary_roots;
extern std::stack<Object*> grey_list;
extern MemoryManager<Mallocator> memory_manager;

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

void mark_temporaries();
void mark_object(Object* obj);
void mark_black(Object* obj);

template<typename Key,
    typename T,
    typename Hash = std::hash<Key>,
    typename KeyEqual = std::equal_to<Key>,
    typename A = std::allocator<TableEntry<Key, T>>>
void mark_table(HashTable<Key, T, Hash, KeyEqual, A> table) {
    for (auto& entry: table) {
        memory::mark_object(entry.key);
        entry.value.mark( );
    }
}


}    // end of namespace memory


#endif    // CLOXPP_MEMORY_HPP
