//
// Created by Johan Ericsson on 2023-05-09.
//

#include <cstring>
#include "object.hpp"

StringObject* str_from_chars(const char *chars, uint length) {
    char* cstr = allocate<char>(length + 1);
    memcpy(cstr, chars, length);
    cstr[length] = '\0';
    return allocate_string(cstr, length);
}

StringObject* allocate_string(char *chars, uint length) {
    auto* string = allocate_object<StringObject>(OBJ_STRING);
    string->length = length;
    string->chars = chars;
    return string;
}

StringObject* concatenate(StringObject *str1, StringObject *str2) {
    size_t length = str1->length + str2->length;
    char* chars = allocate<char>(length+1);
    memcpy(chars, str1->chars, str1->length);
    memcpy(chars+str1->length, str2->chars, str2->length);
    chars[length]='\0';
    return allocate_string(chars, length);
}
