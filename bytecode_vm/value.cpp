//
// Created by Johan Ericsson on 2023-04-17.
//

#include "value.hpp"

void print_value(Value v) {
    switch(v.value_type()) {
        case VAL_BOOL:
            std::cout << (v.bool_value() ? "true" : "false");
            break;
        case VAL_NIL:
            std::cout << "nil";
            break;
        case VAL_NUMBER:
            //std::cout << "\t" << std::right << std::setw(5) << v.number_value() << std::endl;
            std::cout << std::setw(5) << v.number_value();
            break;
    }
}


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
    return Value(v1.number_value() / v2.number_value());
}

