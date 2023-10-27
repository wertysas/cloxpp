//
// Created by Johan Ericsson on 9/19/23.
//

#include <iostream>
#include "object.hpp"


template<typename T>
void print_class_info() {
std::cout << "Class: " << typeid(T).name() << "\tsize: " << sizeof(T)
<< "bytes" << "\talignment" << alignof(T) << std::endl;
}


int main() {
    print_class_info<StringObject>();
    print_class_info<FunctionObject>();
    print_class_info<NativeObject>();
    print_class_info<ClosureObject>();
    print_class_info<UpValueObject>();

    return 0;
};