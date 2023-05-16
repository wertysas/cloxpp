//
// Created by Johan Ericsson on 2023-05-16.
//

#ifndef CLOXPP_ALLOCATOR_HPP
#define CLOXPP_ALLOCATOR_HPP

#include <cstddef>

namespace memory {

template<typename T>
class Allocator {
    public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
};

}

#endif //CLOXPP_ALLOCATOR_HPP
