//
// Created by Johan Ericsson on 4/19/23.
//

#ifndef CLOXPP_STACK_HPP
#define CLOXPP_STACK_HPP


#include <cstdlib>
#include <iostream>
#include <cassert>

template <typename T, size_t MAXSIZE=2048>
class StaticStack {
public:
    void reset();
    T* top() { return top_; }
    T* first() { return elements_; }
    T& operator[](size_t idx) const;
    T& operator[](size_t idx);

    void push(T e);
    T pop();
    void popn(size_t size);
    const T& peek(uint dist);

    class Iterator;
    Iterator begin();
    Iterator end();

private:
    T elements_[MAXSIZE];
    T* top_ = elements_;

};

template<typename T, size_t MAXSIZE>
void StaticStack<T, MAXSIZE>::reset() {
    top_ = elements_;
}

template<typename T, size_t MAXSIZE>
void StaticStack<T, MAXSIZE>::push(T e) {
    *top_ = e;
    top_++;
}

template<typename T, size_t MAXSIZE>
T StaticStack<T, MAXSIZE>::pop() {
    top_--;
    return *top_;
}

template<typename T, size_t MAXSIZE>
void StaticStack<T, MAXSIZE>::popn(size_t n) {
    assert(n <=(top_-elements_));
    top_ -= n;
    return *top_;
}

template<typename T, size_t MAXSIZE>
T& StaticStack<T, MAXSIZE>::operator[](size_t idx) const {
    if (idx < MAXSIZE) {
        return elements_[idx];
    }
    exit(1);
}

template<typename T, size_t MAXSIZE>
T& StaticStack<T, MAXSIZE>::operator[](size_t idx) {
    if (idx < MAXSIZE) {
        return elements_[idx];
    }
    exit(1);
}


template<typename T, size_t MAXSIZE>
typename StaticStack<T, MAXSIZE>::Iterator StaticStack<T, MAXSIZE>::end() {
    return StaticStack::Iterator(elements_);
}

template<typename T, size_t MAXSIZE>
typename StaticStack<T, MAXSIZE>::Iterator StaticStack<T, MAXSIZE>::begin() {
    return StaticStack::Iterator(top_);
}


template <typename T, size_t MAXSIZE>
class StaticStack<T, MAXSIZE>::Iterator {
// using difference_type = T;
// using value_type = T;
// using pointer = const T*;
// using reference = const T&;
//using iterator_category = std::forward_iterator_tag;
public:
    explicit Iterator(T* ptr);
    Iterator& operator++();
    Iterator& operator--();
    Iterator operator++(int);
    Iterator operator--(int);
    T& operator*();
    bool operator==(const Iterator& iter);
    bool operator!=(const Iterator& iter);
private:
    T* ptr_;
};

template<typename T, size_t MAXSIZE>
const T& StaticStack<T, MAXSIZE>::peek(uint dist) {
    return *(top_-1-dist);
}


template<typename T, size_t MAXSIZE>
StaticStack<T, MAXSIZE>::Iterator::Iterator(T *ptr) : ptr_(ptr) {}

template<typename T, size_t MAXSIZE>
typename StaticStack<T, MAXSIZE>::Iterator& StaticStack<T, MAXSIZE>::Iterator::operator++() {
    ptr_++;
    return *this;
}


#endif //CLOXPP_STACK_HPP
