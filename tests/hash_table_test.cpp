//
// Created by Johan Ericsson on 2023-06-16.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "hash_table.hpp"
#include <cstdlib> //size_t, malloc, free
#include <new> // bad_alloc, bad_array_new_length
#include <memory>


template <class T>
struct Mallocator
{
    typedef T value_type;
    Mallocator() noexcept {} //default ctor not required by C++ Standard Library

    // A converting copy constructor:
    template<class U> Mallocator(const Mallocator<U>&) noexcept {}
    template<class U> bool operator==(const Mallocator<U>&) const noexcept
    {
        return true;
    }
    template<class U> bool operator!=(const Mallocator<U>&) const noexcept
    {
        return false;
    }
    T* allocate(const size_t n) const;
    void deallocate(T* const p, size_t) const noexcept;
};

template <class T>
T* Mallocator<T>::allocate(const size_t n) const
{
    if (n == 0)
    {
        return nullptr;
    }
    if (n > static_cast<size_t>(-1) / sizeof(T))
    {
        throw std::bad_array_new_length();
    }
    void* const pv = malloc(n * sizeof(T));
    if (!pv) { throw std::bad_alloc(); }
    return static_cast<T*>(pv);
}

template<class T>
void Mallocator<T>::deallocate(T * const p, size_t) const noexcept
{
    free(p);
}

TEST_CASE("Non Pointer Type Hash Table") {
    HashTable<int, int, std::hash<int>, Mallocator<TableEntry<int, int>>> table{ };
    SUBCASE("insertion") {
        table.insert(1, 1);
        table.insert(2, 2);
    }

    SUBCASE("contains") {
        REQUIRE(table.contains(1));
        REQUIRE(table.contains(2));
    }

    SUBCASE("accessing elements") {
        REQUIRE(table[1].value == 1);
        REQUIRE(table[2].value == 2);
    }
    SUBCASE("entry type status") {
            for (auto entry: table) {
                if (entry.key == 1 || entry.key == 2) {
                    REQUIRE(entry.type() == EntryType::USED);
                } else {
                    REQUIRE(entry.type() == EntryType::EMPTY);
                }
            }
    }
}
