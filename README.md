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

An example output from a Macbook Pro M1 running the command above is given below:
```
Lox interpreter benchmark 2024-02-07 23:38:53
interpreter: '/path/to/cloxpp/cmake-build-release/cloxpp'
OS: 'Darwin'
processor: 'arm'
iterations: 5
comment:
--------------------------------------------------------------------------------
benchmark: binary_trees                  completed [5/5]
mean: 5.71 stdev: 0.10 (0.02 means)
--------------------------------------------------------------------------------
benchmark: equality                      completed [5/5]
mean: 1.58 stdev: 0.00 (0.00 means)
--------------------------------------------------------------------------------
benchmark: fib                           completed [5/5]
mean: 1.05 stdev: 0.00 (0.00 means)
--------------------------------------------------------------------------------
benchmark: instantiation                 completed [5/5]
mean: 2.34 stdev: 0.02 (0.01 means)
--------------------------------------------------------------------------------
benchmark: invocation                    completed [5/5]
mean: 0.43 stdev: 0.02 (0.04 means)
--------------------------------------------------------------------------------
benchmark: method_call                   completed [5/5]
mean: 0.18 stdev: 0.00 (0.00 means)
--------------------------------------------------------------------------------
benchmark: properties                    completed [5/5]
mean: 0.60 stdev: 0.00 (0.01 means)
--------------------------------------------------------------------------------
benchmark: string_equality               completed [5/5]
mean: 1.16 stdev: 0.00 (0.00 means)
--------------------------------------------------------------------------------
benchmark: trees                         completed [5/5]
mean: 1.23 stdev: 0.01 (0.01 means)
--------------------------------------------------------------------------------
benchmark: zoo                           completed [5/5]
mean: 0.42 stdev: 0.01 (0.02 means)
--------------------------------------------------------------------------------
benchmark: zoo_batch                     completed [5/5]
mean: 10.00 stdev: 0.00 (0.00 means)
--------------------------------------------------------------------------------
binary_trees,5.77102,5.68022,5.7443,5.75997,5.54397,5.80912
equality,1.57921,1.5796,1.58917,1.58043,1.57806,1.58061
fib,1.06158,1.0437,1.04597,1.04559,1.04751,1.05305
instantiation,2.55119,2.36059,2.31058,2.35212,2.32389,2.32969
invocation,0.45433,0.411908,0.447403,0.412232,0.445396,0.438701
method_call,0.184158,0.182917,0.182833,0.183989,0.18289,0.182878
properties,0.61554,0.606473,0.598351,0.598284,0.597233,0.597744
string_equality,1.15589,1.1579,1.15539,1.15656,1.15382,1.15451
trees,1.25125,1.23678,1.23867,1.22134,1.23836,1.21833
zoo,0.409465,0.412621,0.412764,0.409257,0.412157,0.43127
zoo_batch,10.0008,10.0023,10.0025,10.0009,10.0006,10.0021
```

