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

## Installation
The build system is cmake ...
```
cmake ...
```

## Usage
The VM can be run as an interpreter in a shell by the command
```
./cloxpp
```
Alternatively a lox script can be executed by the command:
```
./cloxpp [script name]
```
