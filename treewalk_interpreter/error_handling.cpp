//
// Created by Johan Ericsson on 2023-01-26.
//

#include <iostream>

#include "error_handling.hpp"


void ErrorReporter::error(uint line_nr, std::string message) {
    report(line_nr, "", message);
}

void ErrorReporter::report(uint line_nr, std::string where, std::string message) {
    std::cerr << "[lines " << line_nr << "] Error" << where << ": " << message << std::endl;
    has_error = true;
}
