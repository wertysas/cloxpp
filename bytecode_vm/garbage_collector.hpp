//
// Created by Johan Ericsson on 2023-05-10.
//

#ifndef CLOXPP_GARBAGE_COLLECTOR_HPP
#define CLOXPP_GARBAGE_COLLECTOR_HPP


struct Object;

class GarbageCollector {
    public:
    void add_object(Object* object);
    private:
    Object*  objects;
};


#endif //CLOXPP_GARBAGE_COLLECTOR_HPP
