//
// Created by Johan Ericsson on 2023-05-09.
//

#ifndef CLOXPP_OBJECT_HPP
#define CLOXPP_OBJECT_HPP

#include "common.hpp"
#include "memory/memory.hpp"
#include "chunk.hpp"
#include <cstring>

using HashType = uint32_t;


struct Object {
    ObjectType type;
    Object* next;
    explicit Object(ObjectType object_type)
        : type(object_type), next(nullptr) {}
};

// This object owns the memory it's chars member points to
struct StringObject : public Object {
    size_t length;
    char* chars;
    uint32_t hash;    // implement through mixin/CRTP?

    // memory of chars must be allocated and freed
    StringObject(const char* str_chars, uint str_len);
    ~StringObject();

    StringObject& operator=(StringObject&& string_object) noexcept;

    void* operator new(size_t);
    void operator delete(void* p);
};

struct StringHash {
    constexpr HashType operator( )(StringObject* s) const { return s->hash; }
};

struct StringEqual {
    bool operator( )(StringObject* s1, StringObject* s2) {
        return strcmp(s1->chars, s2->chars) == 0;
    }
};

StringObject* str_from_chars(const char* chars, uint length);
StringObject* allocate_string(char* chars, uint length, HashType hash);
StringObject* concatenate(StringObject* str1, StringObject* str2);
StringObject* move_string(StringObject* str);
// FNV-1a
HashType hash_string(const char* str, uint length);


struct FunctionObject : public Object {
    uint arity;
    uint16_t upvalue_count;
    Chunk chunk;
    StringObject* name;

    FunctionObject( )
        : Object(OBJ_FUNCTION), arity(0), upvalue_count(0), chunk( ),
          name(nullptr) {}

    void* operator new(size_t);
    void operator delete(void* p);
};

struct UpValueObject : public Object {
    Value* location;
    Value closed;
    UpValueObject* next_upvalue;

    explicit UpValueObject(Value* loc)
        : Object(OBJ_UPVALUE), location(loc), closed( ), next_upvalue(nullptr) {
    }

    void* operator new(size_t);
    void operator delete(void* p);
};

struct ClosureObject : public Object {
    FunctionObject* function;
    UpValueObject** upvalues;
    uint upvalue_count;

    explicit ClosureObject(FunctionObject* fn)
        : Object(OBJ_CLOSURE), function(fn), upvalue_count(fn->upvalue_count) {
        upvalues = memory::allocate<UpValueObject*>(upvalue_count);
        for (uint i = 0; i < upvalue_count; i++) {
            upvalues[i] = nullptr;
        }
    }
    ~ClosureObject( ) { memory::free<UpValueObject*>(upvalues); }
    void* operator new(size_t);
    void operator delete(void* p);
};


typedef Value (*NativeFunction)(uint arg_count, Value* args);
struct NativeObject : public Object {
    NativeFunction function;
    explicit NativeObject(NativeFunction fn)
        : Object(OBJ_NATIVE), function(fn) {}
    void* operator new(size_t);
    void operator delete(void* p);
};

template<typename T>
T* allocate_object( ) {
    size_t size = sizeof(T);
    Object* object = reinterpret_cast<Object*>(reallocate(nullptr, 0, size));

    // Updating object list used to keep track of objects allocated
    object->next = memory::objects;
    memory::objects = object;

    return reinterpret_cast<T*>(object);
}


#endif    // CLOXPP_OBJECT_HPP
