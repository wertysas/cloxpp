//
// Created by Johan Ericsson on 2023-05-10.
// Linear probing Hash Map
//

#ifndef CLOXPP_HASH_TABLE_HPP
#define CLOXPP_HASH_TABLE_HPP

#include <type_traits>

#include "common.hpp"
#include "value.hpp"


#define HASH_TABLE_MAX_LOAD 0.75      // TODO: move to template parameter?
#define HASH_TABLE_GROWTH_FACTOR 2    // TODO: move to template parameter?


////////////////////////////////////////////////////////////////////////////////
// Concepts
////////////////////////////////////////////////////////////////////////////////

/*
 * Allocator concept
 */
template<typename T>
concept Allocator = requires(T t) {
    { t.allocate( ) } -> std::same_as<void*>;
    { t.deallocate( ) } -> std::same_as<void>;
};

/*
 * Pointer Type Concept
 */
template<typename T>
concept Pointer = std::is_pointer_v<T>;

////////////////////////////////////////////////////////////////////////////////
// Table Entry, which is specialized for pointer types
////////////////////////////////////////////////////////////////////////////////
template<typename Key, typename T, typename = void>
struct TableEntry {
    enum class EntryType {
        EMPTY,
        USED,
        DELETED,
    };
    EntryType type;
    Key key;
    T value;
    TableEntry( ) : type(EntryType::EMPTY), key(Key{ }), value(T( )) {}
};

template<typename Key, typename T>
struct TableEntry<Key, T, std::enable_if_t<std::is_pointer_v<Key>>> {
    Key key;
    union {
        T value;
        bool deleted;
    };
    TableEntry( ) : key(nullptr), value(T{ }) {}
};


template<typename Key,
         typename T,
         typename Hash = std::hash<Key>,
         Allocator A = std::allocator<TableEntry<Key, T>>>
class HashTable {
    public:
    // typedefs
    using key_type = Key;
    using value_type = T;
    using entry_type = TableEntry<Key, T>;
    using allocator_type = A;

    // ctors and memory modifying behaviour
    HashTable( ) : count_(0), capacity_(0), entries_(nullptr) {}
    ~HashTable( ) { A::deallocate(entries_); }
    void reset( );
    void free_storage( );
    void resize_table( );

    // iterators

    // size related
    inline bool empty( ) const { return count_ == 0; }
    inline uint size( ) const { return count_; }
    inline uint count( ) const { return count_; }
    inline uint max_size( ) const { return capacity_; }
    inline bool check_capacity( ) const {
        return (count_ + 1 < capacity_ * HASH_TABLE_MAX_LOAD);
    }

    // Lookup and element insertion/deletion
    void insert(const entry_type& entry);     // inserts element
    void erase(const Key& key);    // deletes element
    bool contains(const Key& key);    // checks if the hash table contains a
                                      // value with given key
    entry_type* find(const Key& key);
    inline void operator[](const entry_type& entry) { insert(entry); };
    inline entry_type* operator[](const Key& key) { return find(key); }

    private:
    uint count_;
    uint capacity_;
    TableEntry<Key, T>* entries_;
};

template<typename Key, typename T, typename Hash, Allocator A>
void HashTable<Key, T, Hash, A>::insert(const entry_type& entry) {
    if (!check_capacity( ))    // check if table needs 2 grow
        resize_table( );

    entry_type* entry_ = find(entry.key);
    if constexpr (std::is_pointer_v<key_type>) {    // pointer type
                                                    // specialization
        if (entry_->key == nullptr)
            count_++;
    }
    if constexpr (!std::is_pointer_v<key_type>) {    // non pointer type
                                                     // specialization
        if (entry.value == entry_type::EntryType::UNUSED)
            count_++;
    }
    entry_ = entry;    // default copy assignment operator
}

template<typename Key, typename T, typename Hash, Allocator A>
void HashTable<Key, T, Hash, A>::erase(const HashTable::key_type& key) {
   entry_type* entry = find(key);
   if constexpr (std::is_pointer_v<key_type>) {    // pointer type
       if (entry->key == nullptr)
           return;
       entry = entry_type(); // Tombstone, by default constructed tpye
   }

}

template<typename Key, typename T, typename Hash, Allocator A>
bool HashTable<Key, T, Hash, A>::contains(const Key& key) {
    return find(key) == nullptr;
}

template<typename Key, typename T, typename Hash, Allocator A>
TableEntry<Key, T>* HashTable<Key, T, Hash, A>::find(Key const& key) {
    uint idx = Hash(key) % capacity_;    // TODO: maybe switch 2 fibonacci
    for (;;) {
        entry_type entry = &entries_[idx];
        if constexpr (std::is_pointer_v<Key>) {    // pointer type
                                                   // specialization
            if (entry.key == key || entry.key == nullptr) {
                return entry;
            }
        }
        if constexpr (!std::is_pointer_v<Key>) {    // non pointer type
                                                    // specialization
            if (entry.key == key ||
                entry.type == entry_type::EntryType::EMPTY) {
                return entry;
            }
        }
        idx = (idx + 1) % capacity_;    // FIXME: modulo inside "hot" for loop
    }
}

template<typename Key, typename T, typename Hash, Allocator A>
void HashTable<Key, T, Hash, A>::reset( ) {
    count_ = 0;
    capacity_ = 0;
    entries_ = nullptr;
}

template<typename Key, typename T, typename Hash, Allocator A>
void HashTable<Key, T, Hash, A>::free_storage( ) {
    memory::free_array<TableEntry>(entries_, capacity_);
    reset( );
}
template<typename Key, typename T, typename Hash, Allocator A>
void HashTable<Key, T, Hash, A>::resize_table( ) {
    capacity_ = HASH_TABLE_GROWTH_FACTOR * capacity_;
    entry_type* entries = allocator_type::allocate(capacity_);
    for (int i = 0; i < capacity_; ++i) {
        entries[i] = entry_type( );
    }

    for (uint i = 0; i < capacity_; ++i) {
        entry_type& entry = entries_[i];
        if constexpr (std::is_pointer_v<key_type>) {
            if (entry.key == nullptr)
                continue;
        }
        if constexpr (std::is_pointer_v<key_type>) {
            if (entry.type == entry_type::EntryType::EMPTY)
                continue;
        }
        entry_type* dest = find(entry.key);
        *dest = entry;
    }

    allocator_type::deallocate(entries_);
    entries_ = entries;
}

#endif    // CLOXPP_HASH_TABLE_HPP
