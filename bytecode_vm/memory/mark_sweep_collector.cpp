//
// Created by Johan Ericsson on 2023-08-30.
//

#include "mark_sweep_collector.hpp"
#include "virtual_machine.hpp"


#ifdef DEBUG_LOG_GC
#include <iostream>
#endif

namespace memory {

void MarkSweepCollector::collect( ) {
#ifdef DEBUG_LOG_GC
    std::cout << "-- gc begin\n";
#endif


#ifdef DEBUG_LOG_GC
    std::cout << "-- gc end\n";
#endif
}

void MarkSweepCollector::mark_roots( ) {
    vm_->mark_stack();
    vm_->mark_call_frames();
    vm_->mark_upvalues();
    vm_->mark_globals( );

}


}    // namespace memory