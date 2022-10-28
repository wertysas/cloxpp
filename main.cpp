//
// Created by Johan Ericsson on 2022-10-26.
//

#include <iostream>

void run_file(char*);

int main(int argc, char* argv[]) {
    if (argc == 1) {
        run_file(argv[1]);
    } else if (argc > 0) {
        std::cout << "Usage: cloxpp [script]" << std::endl;
        return 64;
    } else {
        run_prompt();
    }
}


void run_file(char* file_path) {
    std::string path(file_path); // create std::string from C-style char* array string representation
}