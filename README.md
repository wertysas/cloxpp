# cLoxpp
A C++ implementation of a Lox compiler & VM & bytecode interpreter based on Bob Nystrom's book Crafting Interpreters ([craftinginterpreters.com](https://craftinginterpreters.com/))
with further optimizations and extensions including:

* Extended bytecode, which yields increased performance and adds support for compiling more constants.
* Custom Memory Manager with custom allocator support.


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

**cLoxpp is fully functioning and all language features of Lox are supported**

cloxpp passes all unit tests from [munificient/craftinginterpreters](https://github.com/munificent/craftinginterpreters#testing)
(except 2 which it's expected to fail, since it supports larger constant pools for chunks than the interpreter from craftingerpreters).
**Note that the unit tests requires an older (dart 2.X) than the default version of the Dart SDK to run**.

| OS | Tests Passed* |
| --- |---------------|
| Mac OS | 244/244       |
| Linux | 244/244       |
| Windows | Not tested    |

*There are 246 unit tests in the dart test suite but two of them tests
for correct error messages when chunks are too large, which is chunks with
more than 256 elements. However, cloxpp supports *long* chunks
with up to 2^24=16,777,216 elements, thus it "fails" two tests,
which is expected and not included in the table.

## Installation
cLoxpp uses CMake as it's build system. The target to build is cloxpp from the top level
[CMakeLists.txt](CMakeLists.txt). Example commands to build cloxpp
for Mac OS, Linux, and Windows are given below.

### Building on Linux and Mac OS

If you want to build the cloxpp on linux the easiest way is to
navigate to the cloxpp directory and execute the command
```
cmake -DCMAKE_BUILD_TYPE=Release -S . -B cmake-build-release
```
followed by the command
```
cmake --build cmake-build-release --target cloxpp
```
The cloxpp executable is then found at ``cmake-build-release/cloxpp``


**Alternatively, it's possible to build cloxpp from outside the source directory**
by using the same cmake commands as above, but in addition also specifying the paths
to the build directory
```
cmake -DCMAKE_BUILD_TYPE=Release -S /path/to/cloxpp/source/directory -B /path/to/build/directory
```
followed by the command
```
cmake --build /path/to/build/directory --target cloxpp
```


### Building on Windows
On windows the program can be built using the command
```
cmake -S /path/to/source/directory/ -B /path/to/build/directory
```
followed by
```
cmake --build /path/to/build-dir --config Release --target cloxpp
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

### Benchmarking and Performance
The [benchmark](/benchmark) directory contains a benchmarking
script ``benchmark.py`` written in Python.
The script can be run  without any default arguments as:
```
python benchmark.py
```
This will execute each lox script in the [benchmarks](/benchmark/benchmarks)
directory 6 times and display mean and standard deviation
of the measurements
(discarding the 1st iteration as a warmup iteration for
every new benchmark).

The ``benchmark.py`` script support functionality for writing the output
file 

An example output from a Linux Host (AMD 5950X) running the command above is given below:

```
Lox interpreter benchmark 2024-02-08 22:00:14
interpreter: '/home/tailor/code/compilers/cloxpp/cmake-build-release/cloxpp'
OS: 'Linux'
processor: 'x86_64'
iterations: 5
comment:
--------------------------------------------------------------------------------
benchmark: binary_trees                  completed [5/5]
mean: 5.31 stdev: 0.08 (0.02 means)
--------------------------------------------------------------------------------
benchmark: equality                      completed [5/5]
mean: 1.73 stdev: 0.08 (0.05 means)
--------------------------------------------------------------------------------
benchmark: fib                           completed [5/5]
mean: 1.08 stdev: 0.02 (0.02 means)
--------------------------------------------------------------------------------
benchmark: instantiation                 completed [5/5]
mean: 2.54 stdev: 0.03 (0.01 means)
--------------------------------------------------------------------------------
benchmark: invocation                    completed [5/5]
mean: 0.44 stdev: 0.07 (0.15 means)
--------------------------------------------------------------------------------
benchmark: method_call                   completed [5/5]
mean: 0.24 stdev: 0.01 (0.02 means)
--------------------------------------------------------------------------------
benchmark: properties                    completed [5/5]
mean: 0.66 stdev: 0.01 (0.02 means)
--------------------------------------------------------------------------------
benchmark: string_equality               completed [5/5]
mean: 1.24 stdev: 0.03 (0.02 means)
--------------------------------------------------------------------------------
benchmark: trees                         completed [5/5]
mean: 1.23 stdev: 0.02 (0.01 means)
--------------------------------------------------------------------------------
benchmark: zoo                           completed [5/5]
mean: 0.46 stdev: 0.01 (0.02 means)
--------------------------------------------------------------------------------
benchmark: zoo_batch                     completed [5/5]
mean: 10.00 stdev: 0.00 (0.00 means)

```
