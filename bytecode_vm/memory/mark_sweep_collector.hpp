//
// Created by Johan Ericsson on 2023-08-30.
// The one thing which one must keep in mind is that
// allocations in ctor's can trigger gc's for untracked objects
//

#ifndef CLOXPP_MARK_SWEEP_COLLECTOR_HPP
#define CLOXPP_MARK_SWEEP_COLLECTOR_HPP

#include <vector>

class VirtualMachine;
class Parser;
class Object;

namespace memory {

class MarkSweepCollector {
    public:
    MarkSweepCollector() : vm_(nullptr), parser_(nullptr) {};
    void set(VirtualMachine* vm) { vm_ = vm; }
    void set(Parser* parser) { parser_ = parser; }
    void collect( );

    private :
    VirtualMachine* vm_ = nullptr;
    Parser* parser_ = nullptr;
    void mark_roots();
    void trace_references( );
    void sweep();
};

}    // namespace memory

#endif    // CLOXPP_MARK_SWEEP_COLLECTOR_HPP
