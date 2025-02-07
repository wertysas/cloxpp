//
// Created by Johan Ericsson on 2023-04-17.
//

#include <cstring>
#include "value.hpp"
#include "object.hpp"


////////////////////////////////////////////////////////////////////////////////
// Arithmetic Operators
////////////////////////////////////////////////////////////////////////////////

Value operator+(const Value& v1, const Value& v2) {
    return Value(v1.number_value( ) + v2.number_value( ));
}

Value operator-(const Value& v1, const Value& v2) {
    return Value(v1.number_value( ) - v2.number_value( ));
}

Value operator/(const Value& v1, const Value& v2) {
    return Value(v1.number_value( ) / v2.number_value( ));
}

Value operator*(const Value& v1, const Value& v2) {
    return Value(v1.number_value( ) * v2.number_value( ));
}

bool operator==(const Value& v1, const Value& v2) {
    if (v1.value_type( ) != v2.value_type( ))
        return false;

    switch (v1.value_type( )) {
    case VAL_BOOL:
        return v1.bool_value( ) == v2.bool_value( );
    case VAL_NIL:
        return true;
    case VAL_NUMBER:
        return v1.number_value( ) == v2.number_value( );
    case VAL_OBJ:
        if (v1.is_string() && v2.is_string()) {
            auto* str1 = v1.string( );
            auto* str2 = v2.string( );
            return str1->length == str2->length &&
                   (memcmp(str1->chars, str2->chars, str1->length) == 0);
        }
        else return v1.object_value() == v2.object_value();
    }
    return false;
}

bool operator!=(const Value& v1, const Value& v2) {
    return !(v1 == v2);
}

////////////////////////////////////////////////////////////////////////////////
// Comparison operators for numeric types where check for value_type.numeric()
// type checks are done in VM runtime
////////////////////////////////////////////////////////////////////////////////
bool operator<(const Value& v1, const Value& v2) {
    return v1.number_value( ) < v2.number_value( );
}

bool operator<=(const Value& v1, const Value& v2) {
    return v1.number_value( ) <= v2.number_value( );
}

bool operator>(const Value& v1, const Value& v2) {
    return v1.number_value( ) > v2.number_value( );
}

bool operator>=(const Value& v1, const Value& v2) {
    return v1.number_value( ) >= v2.number_value( );
}

void print_value(Value value) {
    switch (value.value_type( )) {
    case VAL_BOOL:
        std::cout << (value.bool_value( ) ? "true" : "false");
        break;
    case VAL_NIL:
        std::cout << "nil";
        break;
    case VAL_NUMBER:
        // std::cout << "\t" << std::right << std::setw(5) <<
        // value.number_value() << std::endl;
        std::cout << value.number_value( );
        break;
    case VAL_OBJ:
        print_object(value);
        break;
    }
}

void print_object(Value value) {
    switch (value.object_type( )) {
    case OBJ_STRING:
        if (value.c_string() == nullptr) {
            std::cout << "uninitialied string (nullptr chars)";
            break;
        }
        //std::cout << "\"" << value.c_string( ) << "\"";
        std::cout << value.c_string( );
        break;
    case OBJ_FUNCTION:
        print_function(value.function( ));
        break;
    case OBJ_NATIVE:
        std::cout << "<native fn>";
        break;
    case OBJ_CLOSURE:
        print_function(value.closure( )->function);
        break;
    case OBJ_UPVALUE:
        std::cout << "upvalue";
    case OBJ_CLASS:
        std::cout << value.class_obj()->name->chars;
        break;
    case OBJ_INSTANCE:
        std::cout << value.instance()->klass->name->chars << " instance";
        break;
    case OBJ_BOUND_METHOD:
        print_function(value.bound_method()->method->function);
        break;
    }
}

void print_function(FunctionObject* function) {
    if (function->name == nullptr) {
        std::cout << "<script>";
        return;
    }
    std::cout << "<fn " << function->name->chars << ">";
}
ObjectType Value::object_type( ) const {
    return value_.obj->type;
}

char* Value::c_string( ) const {
    return (static_cast<StringObject*>(value_.obj))->chars;
}

StringObject* Value::string( ) const {
    return static_cast<StringObject*>(value_.obj);
}

FunctionObject* Value::function( ) const {
    return static_cast<FunctionObject*>(value_.obj);
}
NativeFunction Value::native_function( ) const {
    return static_cast<NativeObject*>(value_.obj)->function;
}
ClosureObject* Value::closure( ) const {
    return static_cast<ClosureObject*>(value_.obj);
}
ClassObject* Value::class_obj( ) const {
    return static_cast<ClassObject*>(value_.obj);
}
InstanceObject* Value::instance( ) const {
    return static_cast<InstanceObject*>(value_.obj);
}
BoundMethodObject* Value::bound_method( ) const {
    return static_cast<BoundMethodObject*>(value_.obj);
}

// FIXME: Cant this be cleaned up by using concept + template -> 1 constructor
// instead of 4 even though class is not template?
Value::Value(StringObject* string_obj) : value_type_(VAL_OBJ), value_( ) {
    value_.obj = static_cast<Object*>(string_obj);
}
Value::Value(FunctionObject* function_obj) : value_type_(VAL_OBJ), value_( ) {
    value_.obj = static_cast<Object*>(function_obj);
}
Value::Value(NativeObject* native_obj) : value_type_(VAL_OBJ), value_( ) {
    value_.obj = static_cast<Object*>(native_obj);
}
Value::Value(ClosureObject* native_obj) : value_type_(VAL_OBJ), value_( ) {
    value_.obj = static_cast<Object*>(native_obj);
}
Value::Value(ClassObject* class_object) : value_type_(VAL_OBJ), value_() {
    value_.obj = class_object;
}
Value::Value(InstanceObject* instance) : value_type_(VAL_OBJ), value_() {
    value_.obj = instance;
}
Value::Value(BoundMethodObject* bound_method) : value_type_(VAL_OBJ), value_() {
    value_.obj = bound_method;
}

void Value::mark( ) const {
    if (is_object( )) {
        memory::mark_object(object_value( ));
    }
}


bool is_falsy(Value value) {
    return value.is_nil( ) || (value.is_bool( ) && !value.bool_value( ));
}
