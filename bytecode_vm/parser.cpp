//
// Created by Johan Ericsson on 4/24/23.
//

#include "parser.hpp"

void Parser::error(const char *message) {
    error(previous_, message);
}

void Parser::advance() {

}
