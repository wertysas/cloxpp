//
// Created by Johan Ericsson on 2023-08-30.
//

#include "mark_sweep_collector.hpp"
#include "virtual_machine.hpp"
#include "parser.hpp"

#ifdef DEBUG_LOG_GC
#include <iostream>
#endif

namespace memory {

void MarkSweepCollector::collect( ) {
    if (vm_== nullptr || parser_== nullptr) return;

#ifdef DEBUG_LOG_GC
    std::cout << "-- gc begin\n";
#endif

    mark_roots();
    trace_references();
    // we have no string table!
    sweep();
#ifdef DEBUG_LOG_GC
    std::cout << "-- gc end\n";
#endif
}

void MarkSweepCollector::mark_roots( ) {
    vm_->mark_stack();
    vm_->mark_call_frames();
    vm_->mark_upvalues();
    vm_->mark_globals( );
    parser_->mark_compiler_roots();
}
void MarkSweepCollector::trace_references( ) {
    while (!grey_list.empty()) {
        mark_black(grey_list.top());
        grey_list.pop();
    }
}
void MarkSweepCollector::sweep( ) {
    Object *prev = nullptr, *obj = objects;
    while (obj != nullptr) {
        if (obj->marked) {
            obj->marked = false;
            prev = obj;
            obj = obj->next;
        } else {
            Object* isolated = obj;
            obj = obj->next;
            if (prev != nullptr) {
                prev->next = obj;
            } else {
                objects = obj;
            }
            free_object(isolated);
        }
    }
}


}    // namespace memory