//
// Created by Johan Ericsson on 2023-05-10.
//

#ifndef CLOXPP_TABLE_HPP
#define CLOXPP_TABLE_HPP


#include "common.hpp"
#include "value.hpp"

struct TableEntry {
    StringObject* key;
    Value value;
};

class Table {
    uint count;
    uint capacity;
    TableEntry* entries;
};


#endif //CLOXPP_TABLE_HPP
