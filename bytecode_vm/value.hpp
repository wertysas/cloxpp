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
    inline bool bool_value() const { return value_.boolean; }
    inline double number_value() const { return value_.number; }
    inline bool bool_value() { return value_.boolean; }
    inline double& number_value() { return value_.number; }
    inline bool is_number() const { return value_type_==VAL_NUMBER; }
    inline bool is_bool() const { return value_type_==VAL_BOOL; }
    inline bool is_nil() const { return value_type_==VAL_NIL; }
    inline ValueType value_type() const { return value_type_; }

    friend Value operator+(const Value& v1, const Value& v2);
    friend Value operator-(const Value& v1, const Value& v2);
    friend Value operator/(const Value& v1, const Value& v2);
    friend Value operator*(const Value& v1, const Value& v2);
    friend bool operator==(const Value& v1, const Value& v2);
    friend bool operator!=(const Value& v1, const Value& v2);
    // The 4 comparison operators <, <=, >, >= assumes numeric value type
    // However this type check is done in the runtime of the VM
    friend bool operator<(const Value& v1, const Value& v2);
    friend bool operator<=(const Value& v1, const Value& v2);
    friend bool operator>(const Value& v1, const Value& v2);
    friend bool operator>=(const Value& v1, const Value& v2);

private:
    ValueType value_type_;
    ValueUnion value_;
};


void print_value(Value value);

// Checks whether a Value is falsy, i.e. considered false in a boolean context
// In Lox nil and false are the only falsy values every other value/type evaluates to true.
bool check_falsiness(Value value);


#endif //CLOXPP_VALUE_HPP
