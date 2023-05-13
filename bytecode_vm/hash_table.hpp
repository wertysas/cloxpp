//
// Created by Johan Ericsson on 2023-05-10.
//

#ifndef CLOXPP_HASH_TABLE_HPP
#define CLOXPP_HASH_TABLE_HPP


#include "common.hpp"
#include "value.hpp"

struct TableEntry {
    StringObject* key;
    Value value;
};

class HashTable {
public:
    HashTable() : count(0), capacity(0), entries(nullptr) {}
    void reset();
    void free_storage();
private:
    uint count;
    uint capacity;
    TableEntry* entries;
};


#endif //CLOXPP_HASH_TABLE_HPP
