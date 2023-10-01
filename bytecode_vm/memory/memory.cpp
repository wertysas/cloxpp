//
// Created by Johan Ericsson on 2023-04-03.
//

#include <cstdlib>
#include "memory.hpp"
#include "object.hpp"

namespace memory {

// global linked list to keep track of every allocated object
Object* objects = nullptr;

// Memory manager;
MemoryManager<Mallocator> memory_manager{};

//
std::stack<Object*> grey_list{};

void mark_object(Object* obj) {
    if (obj == nullptr || obj->marked) return;
#ifdef DEBUG_LOG_GC
    std::cout << obj << " marked object ";
    print_value(Value(obj));
    std::cout << std::endl;
#endif
    obj->mark();
    grey_list.push(obj);
}

void mark_black(Object* obj) {
#ifdef DEBUG_LOG_GC
   std::cout << obj << " black-marked object ";
   print_value(Value(obj));
   std::cout << std::endl;
#endif
    switch (obj->type) {
    case OBJ_NATIVE:
    case OBJ_STRING:
        break;
    case OBJ_UPVALUE: {
        auto* upvalue = static_cast<UpValueObject*>(obj);
        upvalue->closed.mark();
        break;
    }
    case OBJ_FUNCTION: {
        auto* function = static_cast<FunctionObject*>(obj);
        mark_object(function->name);
        function->chunk.mark_constants();
        break;
    }
    case OBJ_CLOSURE:
        auto* closure = static_cast<ClosureObject*>(obj);
        mark_object(closure->function);
        for (uint i=0; i<closure->upvalue_count; i++) {
            mark_object(closure->upvalues[i]);
        }
        break;
    }
}

} // end of namespace memory


void free_objects( ) {
    Object* object = memory::objects;
    while (object != nullptr) {
        Object* next = object->next;
        free_object(object);
        object = next;
    }
}

// TODO: This can be done by specific dtors instead as in FunctionObject
void free_object(Object* object) {

    switch (object->type) {
    case OBJ_STRING: {
        delete static_cast<StringObject*>(object);
        break;
    }
    case OBJ_FUNCTION: {
        delete static_cast<FunctionObject*>(object);
        break;
    }
    case OBJ_NATIVE: {
        delete static_cast<NativeObject*>(object);
        break;
    }
    case OBJ_CLOSURE: {
        delete static_cast<ClosureObject*>(object);
        break;
    }
    case OBJ_UPVALUE: {
        delete static_cast<UpValueObject*>(object);
    }
    }
}


