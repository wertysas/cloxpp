//
// Created by Johan Ericsson on 2023-04-21.
//

#include "token.hpp"

const char* token_to_string(TokenType type) {
    return token_types[type];
}