//
// Created by Johan Ericsson on 2023-05-10.
//

#include "garbage_collector.hpp"
#include "object.hpp"

void GarbageCollector::add_object(Object *object) {
    object->next = objects;
    objects = object;
}
