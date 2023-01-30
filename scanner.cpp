//
// Created by Johan Ericsson on 2023-01-26.
//

#include "scanner.hpp"

const std::map<std::string, TokenType> Scanner::keyword_table = {
{"and",    AND},
{"class",  CLASS},
{"else",   ELSE},
{"false",  FALSE},
{"for",    FOR},
{"fun",    FUN},
{"if",     IF},
{"nil",    NIL},
{"or",     OR},
{"print",  PRINT},
{"return", RETURN},
{"super",  SUPER},
{"this",   THIS},
{"true",   TRUE},
{"var",    VAR},
{"while",  WHILE}
};

std::vector<Token> Scanner::scanTokens() {
    while (current < source.length()) {
        start = current;
        scanToken();
    }
    return std::vector<Token>();

}

void Scanner::scanToken() {
    char c = advance();
    switch (c) {

        // Single char tokens
        case '(':
            addToken(LEFT_PAREN);
            break;
        case ')':
            addToken(RIGHT_PAREN);
            break;
        case '{':
            addToken(LEFT_BRACE);
            break;
        case '}':
            addToken(RIGHT_BRACE);
            break;
        case ',':
            addToken(COMMA);
            break;
        case '.':
            addToken(DOT);
            break;
        case '-':
            addToken(MINUS);
            break;
        case '+':
            addToken(PLUS);
            break;
        case ';':
            addToken(SEMICOLON);
            break;
        case '*':
            addToken(STAR);
            break;


        // Operators
        case '!':
            addToken(match('=') ? BANG_EQUAL : BANG);
            break;
        case '=':
            addToken(match('=') ? EQUAL_EQUAL : EQUAL);
            break;
        case '<':
            addToken(match('=') ? LESS_EQUAL : LESS);
            break;
        case '>':
            addToken(match('=') ? GREATER_EQUAL : GREATER);
            break;

        // Ignore whitespaces
        case ' ':
        case '\r':
        case '\t':
            break;
        case   '\n':
            line++;
            break;

        // Special character /
        case '/':
            if (match('/')) {
                // A comment goes until the end of the line.
                while (peek() != '\n' && !end_of_source()) advance();
            } else {
                addToken(SLASH);
            }
            break;


        // String literals
        case '"':
            handle_string();
            break;

        // Digits  (char value for 0-9 in ASCII/UTF-8 is 48, ..., 57)
        case 48 ... 57:
            handle_number();
            break;

        // case 'o':
        //     if (match('r')) { addToken(OR); }
        //     break;


        default:
            if (is_alpha(c)) {
                identifier();
            }
            errorReporter.error(line, "Unexpected character.");
            break;
    }
}

char Scanner::advance() {
    return source[current++];
}

void Scanner::addToken(TokenType type) {
    addToken(type, nullptr);
}

void Scanner::addToken(TokenType type, void *literal) {
    string lexeme = source.substr(start, current);
    tokens.emplace_back(type, lexeme, literal, line);
}

bool Scanner::match(char c) {
    if (current >= source.length()) { return false; }
    if (source[current] != c) {  return false; }
    current++;
    return true;
}

char Scanner::peek() const {
    if (end_of_source()) { return '\0'; }
    return source[current];
}

void Scanner::handle_string() {
    //  Find end of string
    while (peek() != '"' && !end_of_source()) {
        if (peek() == '\n') { line++; }
        current++;
    }
    //  If final " not found
    if (end_of_source()) {
        errorReporter.error(line, "Unterminated string.");
        return;
    }

    // TODO: Maybe empty string should be handld differently? Though it is a string now.
    string value = source.substr(start+1, current++); // We skip the double quotes in the str. repr.
    addToken(STRING, &value);

}

void Scanner::handle_number() {
    while (is_digit(peek())) { current++; }

    if (peek() == '.' && is_digit(peek_next())) {
        current++; // Add dot sign in representation
        while (is_digit(peek())) {
            current++;
        }
        double* d = new double;
        *d = (stod(source.substr(start, current)));
        addToken(NUMBER, d);
    }

}

inline bool Scanner::is_digit(char c) {
    return (c >= '0' && c <= '9');
}

char Scanner::peek_next() const {
    if (current+1 >= source.length()) { return '\0'; }
    return source[current+1];
}

bool Scanner::is_alpha(char c) {
    bool ret = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
    return ret;
}

bool Scanner::is_alpha_numeric(char c) {
    return is_alpha(c) || is_digit(c);
}

void Scanner::identifier() {
    while (is_alpha_numeric(peek())) {current++;}

    std::string id = source.substr(start, current);
    TokenType type = Scanner::keyword_table.at(id);
    if



}

