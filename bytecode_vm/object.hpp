//
// Created by Johan Ericsson on 2023-05-09.
//

#ifndef CLOXPP_OBJECT_HPP
#define CLOXPP_OBJECT_HPP

#include "common.hpp"
#include "memory/memory.hpp"
#include "chunk.hpp"
#include "hash_table.hpp"
#include <cstring>

using HashType = uint32_t;


struct Object {
    ObjectType type;
    Object* next;
    bool marked;
    explicit Object(ObjectType object_type) : type(object_type), marked(false) {
        next = memory::objects;
        memory::objects = this;
    }

    inline void mark( ) { marked = true; }
    inline void unmark( ) { marked = false; }
};

std::ostream& operator<<(std::ostream& os, const ObjectType& type);


// This object owns the memory it's chars member points to
struct StringObject : public Object {
    size_t length;
    char* chars;
    uint32_t hash;

    StringObject(const char* str_chars, uint str_len);
    ~StringObject( );

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
    StringObject* name;
    Chunk chunk;

    FunctionObject( )
        : Object(OBJ_FUNCTION), arity(0), upvalue_count(0), name(nullptr),
          chunk( ) {
        memory::temporary_roots.pop_back( );
    }
    explicit FunctionObject(StringObject* name)
        : Object(OBJ_FUNCTION), arity(0), upvalue_count(0), name(name),
          chunk( ) {
        memory::temporary_roots.pop_back( );
    }


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
        : Object(OBJ_CLOSURE), function(fn), upvalue_count(0) {
        memory::temporary_roots.push_back(this);
        upvalues = memory::allocate_array<UpValueObject*>(fn->upvalue_count);
        memory::temporary_roots.pop_back( );
        upvalue_count =
            fn->upvalue_count;    // we modify upvalues after allocation due to
                                  // potential GC trigger
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


struct ClassObject : public Object {
    using table_type = HashTable<StringObject*, Value, StringHash, StringEqual>;
    using entry_type = TableEntry<StringObject*, Value>;

    StringObject* name;
    table_type methods;

    explicit ClassObject(StringObject* name) : Object(OBJ_CLASS), name(name) {}

    void* operator new(size_t);
    void operator delete(void* p);
};

struct InstanceObject : public Object {
    using table_type = HashTable<StringObject*, Value, StringHash, StringEqual>;
    using entry_type = TableEntry<StringObject*, Value>;

    ClassObject* klass;
    table_type fields;

    explicit InstanceObject(ClassObject* klass)
        : Object(OBJ_INSTANCE), klass(klass), fields( ) {}

    void* operator new(size_t);
    void operator delete(void* p);
};


struct BoundMethodObject : public Object {
    Value receiver;
    ClosureObject* method;

    explicit BoundMethodObject(Value receiver, ClosureObject* method)
        : Object(OBJ_BOUND_METHOD), receiver(receiver), method(method) {}

    void* operator new(size_t);
    void operator delete(void* p);
};

template<typename T>
T* allocate_object( ) {
    size_t size = sizeof(T);
    Object* object =
        reinterpret_cast<Object*>(memory::memory_manager.allocate(size));
#ifdef DEBUG_LOG_GC
    std::cout << object << " allocated " << size << " for object type "
              << typeid(T).name( ) << "\n";
#endif
    return reinterpret_cast<T*>(object);
}


#endif    // CLOXPP_OBJECT_HPP
