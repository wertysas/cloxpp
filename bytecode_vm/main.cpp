//
// Created by Johan Ericsson on 2023-04-03.
//

#include <fstream>
#include <sstream>
#include "common.hpp"
#include "dynamic_array.hpp"
#include "chunk.hpp"
#include "debug.hpp"
#include "virtual_machine.hpp"

void repl();

InterpretResult interpret(std::string basicString);

void run_file(char *string);

using std::cin;
using std::cout;
using std::cerr;
using std::endl;

int main(int argc, char* argv[]) {

    VirtualMachine VM{};

    if (argc==1) {
        repl();
    }
    else if (argc==2) {
        run_file(argv[1]);
    } else {
        std::cerr << "Usage cloxpp [path]" << std::endl;
        exit(64);
    }


}

void run_file(char *file_path) {
    std::ifstream fs(file_path);
    if (fs.fail()) { cout << "Failed top open file: " << file_path << endl; }
    std::stringstream file_buffer;
    std::string source = file_buffer.str();
    InterpretResult result = interpret(source);

    if (result == INTERPRET_COMPILE_ERROR) { exit(65); }
    if (result == INTERPRET_RUNTIME_ERROR) { exit(70); }
}

void repl() {
    std::string input_line;
    for (;;) {
       std::cout << "> ";// << std::flush;
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
      interpret(input_line);
    }
}

InterpretResult interpret(std::string basicString) {
    compile
}
