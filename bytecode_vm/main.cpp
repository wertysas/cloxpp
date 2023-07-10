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
#include "compiler.hpp"
#include "memory.hpp"

void repl(VirtualMachine& VM);

InterpretResult interpret(std::string& basicString, VirtualMachine& VM);

void run_file(char* string, VirtualMachine& VM);

using std::cin;
using std::cout;
using std::cerr;
using std::endl;

int main(int argc, char* argv[]) {
    VirtualMachine VM{ };
    if (argc == 1) {
        repl(VM);
    } else if (argc == 2) {
        run_file(argv[1], VM);
    } else {
        std::cerr << "Usage cloxpp [path]" << std::endl;
        exit(64);
    }
    free_objects();
}

void run_file(char* file_path, VirtualMachine& VM) {
    std::ifstream fs(file_path);
    if (fs.fail( )) {
        cout << "Failed top open file: " << file_path << endl;
    }
    std::stringstream file_buffer;
    file_buffer << fs.rdbuf();
    std::string source = file_buffer.str( );
    InterpretResult result = VM.interpret(source);

    if (result == INTERPRET_COMPILE_ERROR) {
        exit(65);
    }
    if (result == INTERPRET_RUNTIME_ERROR) {
        exit(70);
    }
    free_objects();
}

void repl(VirtualMachine& VM) {
    std::cout << "  _        __  __\n"
                 " | |    ___\\ \\/ /\n"
                 " | |   / _ \\\\  / \n"
                 " | |__| (_) /  \\ \n"
                 " |_____\\___/_/\\_\\\n"
                 "                 \n"
                 ""
              << "LoX interpreter version 0.0" << std::endl;
    std::string input_line;
    for (;;) {
        std::cout << "> ";    // << std::flush;
        getline(cin, input_line);
        if (!cin) {
            if (cin.eof( )) {
                cout << "EOF detected, shutting down!" << endl;
            } else {
                cout << "Readline failure, shutting down!" << endl;
            }
            break;
        }
        VM.interpret(input_line);
    }
}

