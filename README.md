# cLoxpp
A C++ implementation of a Lox compiler & VM & bytecode interpreter based on Bob Nystrom's book Crafting Interpreters:
* [craftinginterpreters.com](https://craftinginterpreters.com/)

## Lox Language
The specification of Lox can be found in [chapter 3](https://craftinginterpreters.com/the-lox-language.html#the-standard-library) of Crafting Interpreters.

**TL;DR;** Lox is:
- Dynamically typed (similar to Python) "everything is an object". With 5 built-in data types booleans, numbers (double precision floats), strings, nil
- Garbage Collected
- Supports closures
-  . . .

The following snippet shows a recursive function
to calculate fibonnaci numbers:
```
fun fib(n) {
  if (n < 2) return n;
  return fib(n - 2) + fib(n - 1);
}

print "The 35th fibonnaci number is:";
print fib(35);
```

More examples can be found in the [benchmarks](/benchmark/benchmarks) directory.

## Implementation status 

cLoxpp supports all language features of Lox and passes all unit tests from [munificient/craftinginterpreters](https://github.com/munificent/craftinginterpreters#testing)
(except 2 which it's expected to fail, since it supports larger constant pools for chunks than the interpreter from craftingerpreters).
**Note** that the unit tests requires an older version of the Dart SDK to run.

## Installation
cLoxpp uses CMake as it's build system. The target to build is cloxpp from the top level
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

### Benchmarking Scripts

