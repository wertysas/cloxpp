//
// Created by Johan Ericsson on 2023-01-26.
//

#ifndef CLOXPP_ERROR_HANDLING_HPP
#define CLOXPP_ERROR_HANDLING_HPP

#include <string>

using uint = unsigned int;


class ErrorReporter {
    public:
        bool has_error=false;
        void error(uint line_nr, std::string message);
    private:
        void report(uint line_nr, std::string where, std::string message);

};



#endif //CLOXPP_ERROR_HANDLING_HPP
