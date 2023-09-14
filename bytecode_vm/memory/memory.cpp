//
// Created by Johan Ericsson on 2023-04-03.
//

#include <cstdlib>
#include "memory.hpp"
#include "object.hpp"

// global linked list to keep track of every allocated object
Object* memory::objects = nullptr;

void free_object(Object* object);

// realloc wrapper used for garbage collection purposes which just as
// realloc does:
// 1. if ptr is null  it allocates memory of size new_size (malloc)
// 2. if non-null ptr and new_size 0 it frees the memory
// 3. Else realloc grows/decreases the allocation
void* reallocate(void* ptr, size_t old_size, size_t new_size) {
    if (new_size == 0) {
        free(ptr);
        return nullptr;
    }
    void* res;
    // if (old_size==0) {
    //     res = malloc(new_size);
    //     if (res==nullptr) { exit(1); }
    //     return res;
    // }
    res = realloc(ptr, new_size);
    if (res == nullptr) {
        exit(1);
    }
    return res;
}

void free_objects( ) {
    Object* object = memory::objects;
    while (object != nullptr) {
        Object* next = object->next;
        free_object(object);
        object = next;
    }
}

// TODO: This can be done by specific dtors instead as in FunctionObject
void free_object(Object* object) {
    switch (object->type) {
    case OBJ_STRING: {
        delete static_cast<StringObject*>(object);
        break;
    }
    case OBJ_FUNCTION: {
        delete static_cast<FunctionObject*>(object);
        break;
    }
    case OBJ_NATIVE: {
        delete static_cast<NativeObject*>(object);
        break;
    }
    case OBJ_CLOSURE: {
        delete static_cast<ClosureObject*>(object);
        break;
    }
    case OBJ_UPVALUE: {
        delete static_cast<UpValueObject*>(object);
    }
    }
}
