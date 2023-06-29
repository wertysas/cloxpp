//
// Created by Johan Ericsson on 2023-05-09.
//

#ifndef CLOXPP_OBJECT_HPP
#define CLOXPP_OBJECT_HPP

#include "common.hpp"
#include "memory.hpp"
#include <cstring>

using HashType = uint32_t;

enum ObjectType : uint8_t {
    OBJ_STRING
};

struct Object {
    ObjectType type;
    Object* next;
};

struct StringObject : public Object {
    size_t length;
    char* chars;
    uint32_t hash; // implement through mixin/CRTP?
};


template<typename T>
T* allocate_object(ObjectType object_type) {
    size_t size = sizeof(T);
    Object* object = reinterpret_cast<Object*>(reallocate(nullptr, 0, size));
    object->type = object_type;

    // Updating object list used to keep track of objects allocated
    object->next = memory::objects;
    memory::objects = object;

    return reinterpret_cast<T*>(object);
}

StringObject* str_from_chars(const char* chars, uint length);
StringObject* allocate_string(char* chars, uint length, HashType hash);
StringObject* concatenate(StringObject* str1, StringObject* str2);
StringObject* move_string(StringObject* str);

// FNV-1a
HashType hash_string(const char* str, uint length);

struct StringHash {
 constexpr HashType operator()(StringObject* s) const { return s->hash; }
};

struct StringEqual {
     bool operator()(StringObject* s1, StringObject* s2) {
        return strcmp(s1->chars, s2->chars)==0;
    }
};

#endif //CLOXPP_OBJECT_HPP
