//
// Created by Johan Ericsson on 2023-04-17.
//

#include <cstring>
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
        case VAL_OBJ:
            auto* str1 = v1.string();
            auto* str2 = v2.string();
            return str1->length==str2->length && (memcmp(str1->chars, str2->chars, str1->length)==0);
    }
    return false;
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
            std::cout << value.number_value();
            break;
        case VAL_OBJ:
            print_object(value);
            break;
    }
}

void print_object(Value value) {
    switch (value.object_type()) {
        case OBJ_STRING:
            std::cout << value.c_string();
            break;
    }
}

bool is_falsy(Value value) {
    return value.is_nil() || (value.is_bool() && !value.bool_value());
}

