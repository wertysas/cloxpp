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
#include "parser.hpp"
#include "compiler.hpp"
#include "memory/memory.hpp"

void repl(VirtualMachine& VM, Compiler& compiler);

void run_file(char* string, VirtualMachine& VM, Compiler& compiler);

using std::cin;
using std::cout;
using std::cerr;
using std::endl;

int main(int argc, char* argv[]) {
    VirtualMachine VM{ };
    // memory_manager
    memory::memory_manager.init(&VM);
    Compiler compiler{};

    if (argc == 1) {
        repl(VM, compiler);
    } else if (argc == 2) {
        run_file(argv[1], VM, compiler);
    } else {
        std::cerr << "Usage cloxpp [path]" << std::endl;
        exit(64);
    }
    free_objects();
}

void run_file(char* file_path, VirtualMachine& VM, Compiler& compiler) {
    std::ifstream fs(file_path);
    if (fs.fail( )) {
        cout << "Failed top open file: " << file_path << endl;
    }
    std::stringstream file_buffer;
    file_buffer << fs.rdbuf();
    std::string source = file_buffer.str( );
    FunctionObject* main = compiler.compile(source);
    InterpretResult result = VM.interpret(main);

    if (result == INTERPRET_COMPILE_ERROR) {
        exit(65);
    }
    if (result == INTERPRET_RUNTIME_ERROR) {
        exit(70);
    }
}

void repl(VirtualMachine& VM, Compiler& compiler) {
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
        if (input_line == "exit") {
            cout << "\n LoX exited succesfully!" << endl;
            return;
        }
        FunctionObject* function = compiler.compile(input_line);
        VM.interpret(function);
    }
}

