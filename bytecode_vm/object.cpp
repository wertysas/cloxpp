//
// Created by Johan Ericsson on 2023-05-09.
//

#include <cstring>
#include "object.hpp"

StringObject* str_from_chars(const char *chars, uint length) {
    char* cstr = memory::allocate<char>(length + 1);
    memcpy(cstr, chars, length);
    cstr[length] = '\0';
    HashType hash = hash_string(cstr, length);
    return allocate_string(cstr, length, hash);
}

StringObject* allocate_string(char *chars, uint length, HashType hash) {
    auto* string = allocate_object<StringObject>();
    string->type = OBJ_STRING;
    string->length = length;
    string->chars = chars;
    string->hash = hash;
    return string;
}

StringObject* concatenate(StringObject *str1, StringObject *str2) {
    size_t length = str1->length + str2->length;
    char* chars = memory::allocate<char>(length+1);
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

// TODO: WRITE ONE GENERIC IMPLEMENTATION FOR ALL CLASSES DERIVED FROM OBJECT
// (IF POSSIBLE)
void* FunctionObject::operator new(size_t) {
    return allocate_object<FunctionObject>();
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
