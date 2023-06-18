//
// Created by Johan Ericsson on 2023-05-10.
// Linear probing Hash Map
//

#ifndef CLOXPP_HASH_TABLE_HPP
#define CLOXPP_HASH_TABLE_HPP

#include <type_traits>
#include <memory>

#include "common.hpp"
#include "value.hpp"

#define HASH_TABLE_MAX_LOAD 0.75      // TODO: move to template parameter?
#define HASH_TABLE_GROWTH_FACTOR 2    // TODO: move to template parameter?
#define HASH_TABLE_DEFAULT_SIZE 128

////////////////////////////////////////////////////////////////////////////////
// Concepts
////////////////////////////////////////////////////////////////////////////////
/*
 * Pointer Type Concept
 */
template<typename T>
concept Pointer = std::is_pointer_v<T>;

////////////////////////////////////////////////////////////////////////////////
// Table Entry, which is specialized for pointer types
////////////////////////////////////////////////////////////////////////////////
enum class EntryType {
    EMPTY,
    USED,
    TOMBSTONE,    // i.e. a tombstone entry which
};

template<typename Key, typename T, typename = void>
struct TableEntry {
    EntryType type_;
    Key key;
    T value;
    TableEntry( ) : type_(EntryType::EMPTY), key(Key{ }), value(T( )) {}
    TableEntry(Key key_, T value_)
        : type_(EntryType::USED), key(key_), value(value_) {}
    inline EntryType type( ) const { return type_; }
    inline void set_deleted( ) { type_ = EntryType::TOMBSTONE; }
    TableEntry& operator=(TableEntry const& table_entry) = default;
};

template<typename Key, typename T>
struct TableEntry<Key, T, std::enable_if_t<std::is_pointer_v<Key>>> {
    Key key;
    union {
        T value;
        bool tombstone;
    };
    TableEntry( ) : key(nullptr), tombstone(false) {}
    TableEntry(Key key_, T value_) : key(key_), value(value_) {}
    // inline bool tombstone( ) const {
    //     if (key == nullptr && tombstone == true)
    //         return true;
    //     return false;
    // }
    inline void set_deleted( ) {
        key = nullptr;
        tombstone = false;
    }
    inline EntryType type( ) const {
        if (key == nullptr) {
            if (tombstone)
                return EntryType::TOMBSTONE;
            return EntryType::EMPTY;
        }
        return EntryType::USED;    // Note, it's important
    }
    TableEntry& operator=(TableEntry const& table_entry) {
        key = table_entry.key;
        if (key == nullptr) {    // unused or tombstone
            tombstone = table_entry.tombstone;
        } else {                 // used
            value = table_entry.value;
        }
    }
};


template<typename Key,
         typename T,
         typename Hash = std::hash<Key>,
         typename A = std::allocator<TableEntry<Key, T>>>
class HashTable : public A {    // note, we inherit from allocator for EBCO
    public:
    // typedefs
    using key_type = Key;
    using value_type = T;
    using entry_type = TableEntry<Key, T>;
    using allocator_type = A;

    // ctors and memory modifying behaviour
    HashTable( ) : count_(0), capacity_(0), entries_(nullptr) {}
    ~HashTable( ) { A::deallocate(entries_, capacity_); }
    void reset(size_t size = HASH_TABLE_DEFAULT_SIZE);
    void free_storage( );
    void resize_table( );

    // iterators
    entry_type* begin( ) const { return entries_; }
    entry_type* end( ) const { return entries_ + capacity_; }

    // size related
    inline bool empty( ) const { return count_ == 0; }
    inline uint size( ) const { return count_; }
    inline uint count( ) const { return count_; }
    inline uint max_size( ) const { return capacity_; }
    inline bool has_capacity( ) const {
        return (count_ + 1 < capacity_ * HASH_TABLE_MAX_LOAD);
    }
    inline bool empty() { return count_ == 0; }

    // Lookup and element insertion/deletion
    void insert(const entry_type& entry);    // inserts element
    void insert(key_type const& key, value_type const& value) {
        return insert(entry_type(key, value));
    }
    void erase(const Key& key);       // deletes element
    bool contains(const Key& key);    // checks if the hash table contains a
                                      // value with given key
    entry_type& find(const Key& key) const;
    inline entry_type& operator[](const Key& key) { return find(key); }
    void operator+=(HashTable const& other);

    private:
    allocator_type allocator_{ };
    uint count_;
    uint capacity_;
    TableEntry<Key, T>* entries_;
};

template<typename Key, typename T, typename Hash, typename A>
void HashTable<Key, T, Hash, A>::insert(const entry_type& entry) {
    if (!has_capacity( ))    // check if table needs 2 grow
        resize_table( );

    entry_type& entry_ = find(entry.key);
    if (entry_.type( ) == EntryType::EMPTY)
        count_++;
    entry_ = entry;    // default copy assignment operator
}

template<typename Key, typename T, typename Hash, typename A>
void HashTable<Key, T, Hash, A>::erase(const HashTable::key_type& key) {
    entry_type entry = find(key);
    if (entry.type( ) != EntryType::USED)
        return;
    entry.set_deleted( );
}

template<typename Key, typename T, typename Hash, typename A>
bool HashTable<Key, T, Hash, A>::contains(const Key& key) {
    if (empty( ))
        return false;
    return find(key).type( ) == EntryType::USED;
}


// How to handle when key is  a "bad value"
template<typename Key, typename T, typename Hash, typename A>
TableEntry<Key, T>& HashTable<Key, T, Hash, A>::find(Key const& key) const {
    size_t idx = Hash{ }(key) % capacity_;    // TODO: maybe switch 2 fibonacci
    entry_type* tombstone = nullptr;
    for (;;) {
        entry_type* entry = &entries_[idx];
        EntryType type = entry->type( );
        if (type == EntryType::USED) {
            return *entry;
        } else if (type ==
                   EntryType::EMPTY) {    // return previous tombstone or entry
            return tombstone != nullptr ? *tombstone : *entry;
        }
        if (tombstone == nullptr) {    // else we have a tombstone
            tombstone == entry;
        }
        idx = (idx + 1) % capacity_;    // FIXME: modulo inside "hot" for loop
    }
}

template<typename Key, typename T, typename Hash, typename A>
void HashTable<Key, T, Hash, A>::reset(size_t size) {
    A::deallocate(entries_, capacity_);
    count_ = 0;
    capacity_ = size;
    entries_ = A::allocate(size);
}

template<typename Key, typename T, typename Hash, typename A>
void HashTable<Key, T, Hash, A>::free_storage( ) {
    memory::free_array<TableEntry>(entries_, capacity_);
    reset( );
}
template<typename Key, typename T, typename Hash, typename A>
void HashTable<Key, T, Hash, A>::resize_table( ) {
    size_t old_capacity_ = capacity_;
    capacity_ = (capacity_ == 0) ? HASH_TABLE_DEFAULT_SIZE
                                 : capacity_ * HASH_TABLE_GROWTH_FACTOR;
    entry_type* entries = A::allocate(capacity_);
    for (int i = 0; i < capacity_; ++i) {
        entries[i] = entry_type( );
    }

    for (uint i = 0; i < old_capacity_; ++i) {
        entry_type& entry = entries_[i];
        if constexpr (std::is_pointer_v<key_type>) {
            if (entry.key == nullptr)
                continue;
        }
        if constexpr (std::is_pointer_v<key_type>) {
            if (entry.type_ == EntryType::EMPTY ||
                entry.type == EntryType::TOMBSTONE)
                continue;
        }
        entry_type dest = find(entry.key);
        dest = entry;
    }

    A::deallocate(entries_, old_capacity_);
    entries_ = entries;
}
template<typename Key, typename T, typename Hash, typename A>
void HashTable<Key, T, Hash, A>::operator+=(const HashTable& other) {
    for (entry_type entry: other) {
        insert(entry);
    }
}

#endif    // CLOXPP_HASH_TABLE_HPP
