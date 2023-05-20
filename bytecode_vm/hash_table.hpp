//
// Created by Johan Ericsson on 2023-05-10.
//

#ifndef CLOXPP_HASH_TABLE_HPP
#define CLOXPP_HASH_TABLE_HPP


#include "common.hpp"
#include "value.hpp"

struct TableEntry {
    HashType key;
    Value value;
};

class HashTable {
public:
    void insert(HashType key, Value value);
    bool contains(HashType key);
    TableEntry* find(HashType key);
    void reset();
    void free_storage();
    inline bool empty() const { return count == 0; }
    inline uint size() const { return count; }
    inline uint max_size() const { return capacity; }
    HashTable() : count(0), capacity(0), entries(nullptr) {}
private:
    uint count;
    uint capacity;
    TableEntry* entries;
};


#endif //CLOXPP_HASH_TABLE_HPP
