//
// Created by Johan Ericsson on 2023-04-20.
//

#ifndef CLOXPP_SCANNER_HPP
#define CLOXPP_SCANNER_HPP

#include "common.hpp"
#include "token.hpp"

class Scanner {
    public:
        Scanner(const string& source);
        Token scanToken();
    private:
        const char* start_;
        const char* current_;
        uint line_;
        bool end_of_file();


};


#endif //CLOXPP_SCANNER_HPP
