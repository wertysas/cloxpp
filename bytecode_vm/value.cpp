//
// Created by Johan Ericsson on 2023-04-17.
//

#include "value.hpp"


Value operator+(const Value& v1, const Value& v2) {
        return Value(v1.number_value() + v2.number_value());
}

Value operator-(const Value& v1, const Value& v2) {
    return Value(v1.number_value() - v2.number_value());
}

Value operator/(const Value& v1, const Value& v2) {
    return Value(v1.number_value() / v2.number_value());
}

Value operator*(const Value& v1, const Value& v2) {
    return Value(v1.number_value() * v2.number_value());
}

bool operator==(const Value &v1, const Value &v2) {
    if (v1.value_type() != v2.value_type()) return false;

    switch( v1.value_type()) {
        case VAL_BOOL:
            return v1.bool_value() == v2.bool_value();
        case VAL_NIL:
            return true;
        case VAL_NUMBER:
            return v1.number_value() == v2.number_value();
    }
}

bool operator!=(const Value &v1, const Value &v2) {
    return !(v1==v2);
}

////////////////////////////////////////////////////////////////////////////////
// Operator for numeric types where check for value_type.numeric() has been
// made in VM runtime
////////////////////////////////////////////////////////////////////////////////
bool operator<(const Value &v1, const Value &v2) {
    return v1.number_value() < v2.number_value();
}

bool operator<=(const Value &v1, const Value &v2) {
    return v1.number_value() <= v2.number_value();
}

bool operator>(const Value &v1, const Value &v2) {
    return v1.number_value() > v2.number_value();
}

bool operator>=(const Value &v1, const Value &v2) {
    return v1.number_value() >= v2.number_value();
}

void print_value(Value value) {
    switch(value.value_type()) {
        case VAL_BOOL:
            std::cout << (value.bool_value() ? "true" : "false");
            break;
        case VAL_NIL:
            std::cout << "nil";
            break;
        case VAL_NUMBER:
            //std::cout << "\t" << std::right << std::setw(5) << value.number_value() << std::endl;
            std::cout << std::setw(5) << value.number_value();
            break;
    }
}

bool check_falsiness(Value value) {
    return value.is_nil() || (value.is_bool() && !value.bool_value());
}

