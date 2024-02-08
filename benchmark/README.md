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