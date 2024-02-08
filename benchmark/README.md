# Benchmarks
This directory contains a benchmarking script written in Python
that can be used to benchmark lox interpreters together with some benchmark
programs.


There are currently 11 benchmarks in the [benchmarks](benchmarks) directory
all of which are taken from the reference implementation
of lox for Crafting Interprers:
[https://github.com/munificent/craftinginterpreters](https://github.com/munificent/craftinginterpreters)

## Benchmarking Script ``benchmark.py``
The benchmarking script is written in Python 3, only using the
Python standard library, and
supports several output modes.
More information about the different modes and flags
can  be found by using the command:
```
python benchmark.py -h
```
 
Below is an example of how to run the benchmarking script with a specified lox interpreter,
two chosen benchmarks, and output written to a file named lox_benchmark_BENCHMARK_TIME
in the working directory.

``` 
python benchmark.py -b fib trees -e /path/to/lox/interpreter -o
```

Paths can be given both as relative paths and absolute paths. Furthermore,
it's possible to specify an output file with the ``-o`` flag, for more on this see below.

### Available options
The following options are available when running the script:

| option                        | description                                                                         |
|-------------------------------|-------------------------------------------------------------------------------------|
| **-h, --help**                | show this help message and exit                                                     |
| **-v, --verbose**             | indicates process and gives det  ailed output                                       |
| **-b , --benchmark**          | benchmarks to run                                                                   |
| **-c, --comment**             | benchmark comment added to info header                                              |
| **-e, --executable**          | path to Lox interpeter                                                              |
| **-i, --iterations**          | iterations per benchmark                                                            |
| **-o, --output**              | save output to file                                                                 |
| **-q, -s, --quiet, --silent** | only print benchmark info and csv output to terminal, (same format as output files) |




## Adding new benchmarks
The benchmarking script consider all files with the suffix ``.lox`` in the [benchmarks](benchmarks)
directory to be benchmarking scripts. The execution times are assumed
to be measured inside the program itself and reported by printing the keyword
*elapsed* in the program followed by the recorded time
on the next line.


Here is an example of what a benchmarking script
```title=mybenchmark.lox
//  some lox code not in the benchmark here

var start = clock();
// code to be benchmarked here
var elapsed =  clock() - start;

print "elapsed";
print elapsed;
```

More specifically the benchmarking script works by checking every line
in the output  of lox program for the word *elapsed*.
If a line contains the word elapsed the next line will be parsed as the execution
time.

## Example outputs

An example output from a Macbook Pro M1 running the command
``` 
python benchmark.py -v
```
produces the following output:
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

