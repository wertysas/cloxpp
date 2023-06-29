//
// Created by Johan Ericsson on 2023-06-26.
//

#ifndef CLOXPP_STATIC_ARRAY_HPP
#define CLOXPP_STATIC_ARRAY_HPP

#include "common.hpp"

template <typename T, size_t N>
class StaticArray {
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;

    alignas(T) std::byte elements[sizeof(T)*N];
    public:
    constexpr T* begin()  { return reinterpret_cast<T*>(elements); }
    constexpr const T* begin() const { return reinterpret_cast<T*>(elements); }
    constexpr T* end()  { return reinterpret_cast<T*>(&elements[sizeof(T)*N]); }
    constexpr const T* end() const { return reinterpret_cast<T*>(&elements[sizeof(T)*N]); }
    constexpr reference operator[](size_t index) {
        return begin()[index];
    }
    constexpr const_reference operator[](size_t index) const {
        return begin()[index];
    }
};

#endif    // CLOXPP_STATIC_ARRAY_HPP
