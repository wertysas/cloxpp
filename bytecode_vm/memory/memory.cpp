//
// Created by Johan Ericsson on 2023-04-03.
//

#include "memory.hpp"
#include "object.hpp"

namespace memory {

// global linked list to keep track of every allocated object
Object* objects = nullptr;
// Objects which are being allocated and can get lost if allocation is
// triggered inside ctors
std::list<Object*> temporary_roots{};
// Grey marked work list for GC
std::stack<Object*> grey_list{};
// Memory manager;
MemoryManager<Mallocator> memory_manager{};

void mark_temporaries( ) {
#ifdef DEBUG_LOG_GC
    std::cout << "mark_temporaries begin---------\n";
#endif
    for (auto* obj: temporary_roots) {
        mark_object(obj);
    }
#ifdef DEBUG_LOG_GC
    std::cout << "mark_temporaries end-----------\n";
#endif
}

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
    case OBJ_CLOSURE: {
        auto* closure = static_cast<ClosureObject*>(obj);
        mark_object(closure->function);
        for (uint i = 0; i < closure->upvalue_count; i++) {
            mark_object(closure->upvalues[i]);
        }
        break;
    }
    case OBJ_CLASS: {
        auto* class_obj = static_cast<ClassObject*>(obj);
        mark_object(class_obj->name);
        mark_table(class_obj->methods);
        break;
    }
    case OBJ_INSTANCE: {
        auto* instance = static_cast<InstanceObject*>(obj);
        mark_object(instance);
        mark_table(instance->fields);
        break;
    }
    case OBJ_BOUND_METHOD: {
        auto* bound_method = static_cast<BoundMethodObject*>(obj);
        mark_object(bound_method);
        bound_method->receiver.mark();
        break;

    }
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
        break;
    }
    case OBJ_CLASS: {
        delete static_cast<ClassObject*>(object);
        break;
    }
    case OBJ_INSTANCE: {
        delete static_cast<InstanceObject*>(object);
        break;
    }
    case OBJ_BOUND_METHOD: {
        delete static_cast<BoundMethodObject*>(object);
        break;
    }
    }
}


