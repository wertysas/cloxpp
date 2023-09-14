//
// Created by Johan Ericsson on 2023-09-13.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "../doctest.h"
#include "object.hpp"

TEST_SUITE("Objects") {
    TEST_CASE("StringObject") {
        const char* chars = "axkjlkjhhlaksdlaskdj";
        int len = strlen(chars);
        StringObject* str = new StringObject(chars, len);

        // Check that members correct
        CHECK( str->length == len );
        CHECK(memcmp(str->chars, chars, len)==0);
        CHECK(str->hash == hash_string(chars, len));

        delete str;

    }


}