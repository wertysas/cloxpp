//
// Created by Johan Ericsson on 2022-10-26.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "scanner.hpp"
#include "token.hpp"
#include "error_handling.hpp"

using uint = unsigned int;

using namespace std;


void run_file(char *file_path, ErrorReporter &errorReporter);

void run(string filetext, ErrorReporter &errorReporter);

void run_prompt(ErrorReporter &errorReporter);


int main(int argc, char* argv[]) {
    ErrorReporter errorReporter{};
    if (argc == 2) {
        run_file(argv[1], errorReporter);
        if (errorReporter.has_error) { return 65; }
    } else if (argc > 2) {
        cout << "Usage: cloxpp [script]" << endl;
        return 64;
    } else {
        run_prompt(errorReporter);
    }
    return 0;
}

void run_prompt(ErrorReporter &errorReporter) {
    cout << "CloxPP version 0.1 (prealpha relase)" << endl;
    string input_line;
    while (true) {
        getline(cin, input_line);
        if (!cin) {
            if (cin.eof()) {
                cout << "EOF detected, shutting down!" << endl;
            }
            else {
                cout << "Readline failure, shutting down!" << endl;
            }
            break;
        }
        run(input_line, errorReporter);
        errorReporter.has_error = false;
    }
}


void run_file(char *file_path, ErrorReporter &errorReporter) {
    string path(file_path); // create std::string from C-style char* array string representation
    ifstream fs(path);
    if (fs.fail()) { cout << "Failed top open file: " << path << endl; }
    stringstream file_buffer;
    string filetext = file_buffer.str();
    run(filetext, errorReporter);
}

void run(string source, ErrorReporter &errorReporter) {
    cout << "Input Source String:" <<endl;
    cout << source << endl;
    Scanner scanner(source, errorReporter);
    std::vector<Token> tokens = scanner.scanTokens();
    cout << tokens.size() << endl;
    for (Token t: tokens) {cout << "TOKEN\t lexeme: " << t.lexeme << "\t type: " << token_table.at(t.type) << endl; }
}


