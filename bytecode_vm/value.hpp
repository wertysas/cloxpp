//
// Created by Johan Ericsson on 2023-04-05.
//

#ifndef CLOXPP_VALUE_HPP
#define CLOXPP_VALUE_HPP

#include "common.hpp"


enum ValueType {
    VAL_BOOL,
    VAL_NIL,
    VAL_NUMBER
};

union ValueUnion {
    bool boolean;
    double number;
};

class Value {
    public:
    Value() : value_type_(VAL_NIL), value_() { value_.number=0; } // empty ctor for NIL
    Value(bool value) : value_type_(VAL_BOOL), value_() { value_.boolean=value; }
    Value(double number) : value_type_(VAL_NUMBER), value_() { value_.number=number; }
    private:
    ValueType value_type_;
    ValueUnion value_;
    inline bool bool_value() { return value_.boolean; }
    inline double number_value() { return value_.number; }
    inline bool is_bool() { return value_type_==VAL_BOOL; }
    inline bool is_nil() { return value_type_==VAL_NIL; }
    inline bool is_number() { return value_type_==VAL_NUMBER; }
};



void print_value(Value);


#endif //CLOXPP_VALUE_HPP
