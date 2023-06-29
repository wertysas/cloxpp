//
// Created by Johan Ericsson on 2023-04-21.
//

#include <cstring>
#include "token.hpp"

const char* token_to_string(TokenType type) {
    return token_types[type];
}
bool lexemes_equal(Token token1, Token token2) {
    if (token1.length != token2.length)
        return false;
    return memcmp(token1.start, token2.start, token1.length)==0;
}
