//
// Created by Johan Ericsson on 2023-05-09.
//

#include <cstring>
#include "object.hpp"

std::ostream&  operator<<(std::ostream& os, const ObjectType& type) {
    switch (type) {
    case OBJ_CLOSURE:
        os << "OBJ_CLOSURE";
        break;
    case OBJ_UPVALUE:
        os << "OBJ_UPVALUE";
        break;
    case OBJ_FUNCTION:
        os << "OBJ_FUNCTION";
        break;
    case OBJ_NATIVE:
        os << "OBJ_NATIVE";
        break;
    case OBJ_STRING:
        os << "OBJ_STRING";
        break;
    default:
        os << " error (no type defined)";
        break;
    }
    return os;
}


StringObject* allocate_string(char *chars, uint length, HashType hash) {
    auto* string = allocate_object<StringObject>();
    string->type = OBJ_STRING;
    string->length = length;
    string->chars = chars;
    string->hash = hash;
    return string;
}

// TODO: It's possible this can be cleaned with a new ctor of strobj
StringObject* concatenate(StringObject *str1, StringObject *str2) {
    size_t length = str1->length + str2->length;
    char* chars = memory::allocate_array<char>(length + 1);
    memcpy(chars, str1->chars, str1->length);
    memcpy(chars+str1->length, str2->chars, str2->length);
    chars[length]='\0';
    HashType hash = hash_string(chars, length);
    return allocate_string(chars, length, hash);
}

StringObject* move_string(StringObject *str) {
    return allocate_string(str->chars, str->length, str->hash);
}

// FNV-1a
HashType hash_string(const char *str, uint length) {
    uint32_t hash = 2166136261u;
    for (int i = 0; i < length; i++) {
        hash ^= static_cast<uint8_t>(str[i]);
        hash *= 16777619;
    }
    return hash;
}

StringObject::StringObject(const char* str_chars, uint str_len) : Object(OBJ_STRING), chars(nullptr) {
    // string memory allocation
    memory::temporary_roots.push_back(this);
    char* cstr = memory::allocate_array<char>(str_len + 1);
    memory::temporary_roots.pop_back();
    memcpy(cstr, str_chars, str_len);
    cstr[str_len] = '\0';
    // members
    length = str_len;
    chars = cstr;
    hash  = hash_string(cstr, str_len);

}

StringObject::~StringObject( ) {
    memory::free_array<char>(chars, length+1);
}

// TODO: WRITE ONE GENERIC IMPLEMENTATION FOR ALL CLASSES DERIVED FROM OBJECT
void* StringObject::operator new(size_t) {
    return allocate_object<StringObject>();
}
void StringObject::operator delete(void* p) {
    memory::free<StringObject>(p);
}
void* FunctionObject::operator new(size_t) {
    // this is ugly, but this can't be done in the ctor
    // as the chunk member will trigger an allocation that
    // could possibly trigger a GC cycle
    Object* obj = allocate_object<FunctionObject>();
    memory::temporary_roots.push_back(obj);
    return obj;
}
void FunctionObject::operator delete(void* p) {
    memory::free<FunctionObject>(p);
}
void* NativeObject::operator new(size_t) {
    return allocate_object<NativeObject>();
}
void NativeObject::operator delete(void* p) {
    memory::free<NativeObject>(p);
}
void* ClosureObject::operator new(size_t) {
    return allocate_object<ClosureObject>();
}
void ClosureObject::operator delete(void* p) {
    memory::free<ClosureObject>(p);
}
void* UpValueObject::operator new(size_t) {
    return allocate_object<UpValueObject>();
}
void UpValueObject::operator delete(void* p) {
    memory::free<UpValueObject>(p);
}
