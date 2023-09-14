//
// Created by Johan Ericsson on 2023-09-13.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "../doctest.h"
#include "object.hpp"

TEST_CASE("StringObject") {
    StringObject* str = new StringObject("12345", 5);

}
