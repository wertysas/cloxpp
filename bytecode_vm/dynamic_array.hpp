//
// Created by Johan Ericsson on 2023-04-03.
//

#ifndef CLOXPP_DYNAMIC_ARRAY_HPP
#define CLOXPP_DYNAMIC_ARRAY_HPP


#include "common.hpp"
#include "memory/memory.hpp"

template <typename T>
class DynamicArray {
    private:
        size_t count_;
        size_t capacity_;
        T* elements_;
        void resize();
    public:
        DynamicArray();
        DynamicArray(size_t capacity);
        ~DynamicArray();
        size_t count() const {return count_;}
        size_t capacity() const {return capacity_;}
        void append(T e);
        size_t idx_append(T e);
        T& operator[](size_t idx) const;
        T& operator[](size_t idx);
        T& back();
        T& back() const;
        T* head() {return elements_;}

};

template<typename T>
T &DynamicArray<T>::back() {
    return elements_[count_-1];
}

template<typename T>
T &DynamicArray<T>::back() const {
    return elements_[count_-1];
}

template <typename T>
DynamicArray<T>::DynamicArray() : count_(0), capacity_(INIT_CAPACITY) {
    elements_ = static_cast<T*>(reallocate(nullptr, 0, capacity_*sizeof(T)));
}

template <typename T>
DynamicArray<T>::DynamicArray(size_t capacity) : count_(0), capacity_(capacity) {
    elements_ = static_cast<T*>(reallocate(nullptr, 0, capacity_*sizeof(T)));
}

template <typename T>
DynamicArray<T>::~DynamicArray() {
    reallocate(elements_, sizeof(T)*capacity_, 0);
}

// Function to grow array in size
template<typename T>
inline void DynamicArray<T>::resize() {
    size_t old_size = capacity_*sizeof(T);
    capacity_ *= GROWTH_CONSTANT;
    size_t new_size = capacity_*sizeof(T);
    elements_ = static_cast<T*>(reallocate(elements_, old_size, new_size));
}

template<typename T>
void DynamicArray<T>::append(T e) {
    if (count_+1 > capacity_) {
        resize();
    }
    elements_[count_++] = e;
}

template<typename T>
size_t DynamicArray<T>::idx_append(T e) {
    if (count_+1 > capacity_) {
        resize();
    }
    elements_[count_++] = e;
    return count_ - 1;
}

template<typename T>
T& DynamicArray<T>::operator[](size_t idx) const {
    if (idx < count_) {
        return elements_[idx];
    }
    exit(1);
}

template<typename T>
T& DynamicArray<T>::operator[](size_t idx) {
    if (idx < count_) {
        return elements_[idx];
    }
    exit(1);
}



#endif //CLOXPP_DYNAMIC_ARRAY_HPP
