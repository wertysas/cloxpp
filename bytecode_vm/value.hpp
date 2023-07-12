//
// Created by Johan Ericsson on 2023-04-05.
//

#ifndef CLOXPP_VALUE_HPP
#define CLOXPP_VALUE_HPP

#include "common.hpp"


struct Object;
struct StringObject;
struct FunctionObject;
struct NativeObject;
struct ClosureObject;

enum ObjectType : uint8_t { OBJ_CLOSURE, OBJ_FUNCTION, OBJ_NATIVE, OBJ_STRING };

enum ValueType { VAL_BOOL, VAL_NIL, VAL_NUMBER, VAL_OBJ };


union ValueUnion {
    bool boolean;
    double number;
    Object* obj;
};

class Value {
    typedef Value (*NativeFunction)(uint arg_count, Value* args);

    public:
    Value( ) : value_type_(VAL_NIL), value_( ) {
        value_.number = 0;
    }    // empty ctor for NIL
    Value(bool value) : value_type_(VAL_BOOL), value_( ) {
        value_.boolean = value;
    }
    Value(double number) : value_type_(VAL_NUMBER), value_( ) {
        value_.number = number;
    }
    Value(Object* obj) : value_type_(VAL_OBJ), value_( ) { value_.obj = obj; }
    Value(StringObject* string_obj);
    Value(FunctionObject* function_obj);
    Value(NativeObject* native_obj);
    Value(ClosureObject* closure_object);

    // ValueType accessors
    inline bool bool_value( ) const { return value_.boolean; }
    inline double number_value( ) const { return value_.number; }
    inline bool bool_value( ) { return value_.boolean; }
    inline double number_value( ) { return value_.number; }
    ObjectType object_type( ) const;
    inline Object* object_value( ) const { return value_.obj; }
    StringObject* string( ) const;
    FunctionObject* function( ) const;
    NativeFunction native_function( ) const;
    ClosureObject* closure( ) const;

    // Value type_ checks
    inline ValueType value_type( ) const { return value_type_; }
    inline bool is_number( ) const { return value_type_ == VAL_NUMBER; }
    inline bool is_bool( ) const { return value_type_ == VAL_BOOL; }
    inline bool is_nil( ) const { return value_type_ == VAL_NIL; }
    inline bool is_object( ) const { return value_type_ == VAL_OBJ; }
    inline bool is_object_type(ObjectType obj_type) const {
        return is_object( ) && object_type( ) == obj_type;
    }
    inline bool is_string( ) const { return is_object_type(OBJ_STRING); }
    inline bool is_function( ) const { return is_object_type(OBJ_FUNCTION); }
    inline bool is_native( ) const { return is_object_type(OBJ_NATIVE); }
    inline bool is_closure( ) const { return is_object_type(OBJ_CLOSURE); }
    // String functions
    inline char* c_string( ) const;


    // Binary Operators
    friend Value operator+(const Value& v1, const Value& v2);
    friend Value operator-(const Value& v1, const Value& v2);
    friend Value operator/(const Value& v1, const Value& v2);
    friend Value operator*(const Value& v1, const Value& v2);
    friend bool operator==(const Value& v1, const Value& v2);
    friend bool operator!=(const Value& v1, const Value& v2);
    // The 4 comparison operators <, <=, >, >= assumes numeric value type_
    // However this type_ check is done in the runtime of the VM
    friend bool operator<(const Value& v1, const Value& v2);
    friend bool operator<=(const Value& v1, const Value& v2);
    friend bool operator>(const Value& v1, const Value& v2);
    friend bool operator>=(const Value& v1, const Value& v2);

    friend void print_object(Value value);

    private:
    ValueType value_type_;
    ValueUnion value_;
};


void print_value(Value value);
void print_function(FunctionObject* function);

// Checks whether a Value is falsy, i.e. considered false in a boolean context
// In Lox nil and false are the only falsy values every other value/type_
// evaluates to true.
bool is_falsy(Value value);


#endif    // CLOXPP_VALUE_HPP
