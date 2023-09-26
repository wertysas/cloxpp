//
// Created by Johan Ericsson on 2023-08-30.
//

#ifndef CLOXPP_MARK_SWEEP_COLLECTOR_HPP
#define CLOXPP_MARK_SWEEP_COLLECTOR_HPP


struct VirtualMachine;

namespace memory {

class MarkSweepCollector {
    VirtualMachine* vm_ = nullptr;

    public:
    void init(VirtualMachine* vm) { vm_ = vm; }
    void collect( );
    void mark_roots();
};

}    // namespace memory

#endif    // CLOXPP_MARK_SWEEP_COLLECTOR_HPP
