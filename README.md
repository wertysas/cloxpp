# CLoxPP
A C++ implementation of a Lox compiler & VM & bytecode interpreter based on Bob Nystrom's book Crafting Interpreters:
* [craftinginterpreters.com](https://craftinginterpreters.com/)

## Lox Language
The specification of Lox can be found in [chapter 3](https://craftinginterpreters.com/the-lox-language.html#the-standard-library) of Crafting Interpreters.

**TLDR** Lox is:
- Dynamically typed (similar to Python) "everything is an object". With 5 built-in data types booleans, numbers (double precision floats), strings, nil
- Garbage Collected
- Supports closures
-  . . .

## Implementation status 
Currently the VM supports all but one feature of lox:
- No user defined classes.

CloxPP  passes all unit tests from [munificient/craftinginterpreters](https://github.com/munificent/craftinginterpreters#testing)
(except 2 which it's expected to fail, since it supports larger constant pools for chunks) up to chapter 25.
**Note** that the unit tests requires an older version of the Dart SDK to run.

## Installation
Cloxpp uses CMake as it's build system. The target to build is cloxpp from the top level
[CMakeLists.txt](CMakeLists.txt)

## Usage
The VM can be run as an interpreter in a shell by the command
```
./cloxpp
```
Alternatively a lox script can be executed by the command:
```
./cloxpp [script name]
```
