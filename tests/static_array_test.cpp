//
// Created by Johan Ericsson on 2023-06-26.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "static_array.hpp"


TEST_CASE("static array") {
    StaticArray<int, 4> a;
    a[0] = 1;
    a[1] = 2;
    a[2] = 3;
    a[3] = 4;
    REQUIRE(a[0] == 1);
    REQUIRE(a[1] == 2);
    REQUIRE(a[2] == 3);
    REQUIRE(a[3] == 4);
}
