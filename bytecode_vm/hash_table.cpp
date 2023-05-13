//
// Created by Johan Ericsson on 2023-05-10.
//

#include "hash_table.hpp"

void HashTable::reset() {
    count=0;
    capacity=0;
    entries= nullptr;
}
