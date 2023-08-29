//
// Created by Johan Ericsson on 2023-04-03.
//

#ifndef CLOXPP_MEMORY_HPP
#define CLOXPP_MEMORY_HPP

#include <iostream>
#include "common.hpp"


#define INIT_CAPACITY 8
#define GROWTH_CONSTANT 2


void* reallocate(void* ptr, size_t old_size, size_t new_size);

struct Object;
void free_objects();
void free_object(Object *object);

namespace memory {

template<typename T>
T* allocate(uint count) {
    return reinterpret_cast<T*>(reallocate(nullptr, 0, sizeof(T)*count));
}




extern Object* objects;

template<typename T>
void free(void* ptr) {
    if constexpr (std::is_base_of_v<Object, T>) {

    }
    reallocate(ptr, sizeof(T), 0);
}

template<typename T>
void free_array(void* ptr, size_t size) {
    reallocate(ptr, sizeof(T)*size, 0);
}

}


#endif //CLOXPP_MEMORY_HPP
