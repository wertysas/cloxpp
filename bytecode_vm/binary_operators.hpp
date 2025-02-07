//
// Created by Johan Ericsson on 2023-04-20.
//

#ifndef CLOXPP_BINARY_OPERATORS_HPP
#define CLOXPP_BINARY_OPERATORS_HPP



template<typename T, typename OP>
inline T binary_operator(T a, T b) {
    return OP(a,b);
}

template <typename T>
inline T binary_add(T a, T b) {
    return a+b;
}


template <typename T>
inline T binary_subtract(T a, T b) {
    return a-b;
}

template <typename T>
inline T binary_multiply(T a, T b) {
    return a*b;
}

template <typename T>
inline T binary_divide(T a, T b) {
    return a/b;
}

#endif //CLOXPP_BINARY_OPERATORS_HPP
