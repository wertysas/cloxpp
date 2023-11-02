//
// Created by Johan Ericsson on 11/2/23.
//

#ifndef CLOXPP_CLASS_SCOPE_HPP
#define CLOXPP_CLASS_SCOPE_HPP


class ClassScope {
    ClassScope* enclosing_scope_;
    public:
    ClassScope( ) : enclosing_scope_(nullptr){ };
    ClassScope(ClassScope* enclosing) : enclosing_scope_(enclosing){ };
    ClassScope*& enclosing_scope() { return enclosing_scope_; }
};


#endif    // CLOXPP_CLASS_SCOPE_HPP
