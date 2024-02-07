#!/usr/bin/env python3
"""
Author: Johan Ericsson
"""
import sys
from dataclasses import dataclass
import subprocess
import os
import platform
import pathlib
from pathlib import Path
import argparse
from time import localtime, strftime
from statistics import fmean, stdev

CMAKE_DIR = "cmake-build-release"


@dataclass
class Colors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'


@dataclass
class Benchmark:
    name: str
    path: pathlib.Path



def read_benchmarks(benchmarks_path=None):
    if benchmarks_path is None:
        dir_path = Path(__file__).parent
        bm_path = dir_path.joinpath("benchmarks")
    else:
        bm_path = Path(benchmarks_path)
    benchmarks = {}
    for bmp in bm_path.iterdir():
        if bmp.suffix != ".lox":
            print(f"{Colors.WARNING}Skipping file: '{bmp.name}' (wrong file extension){Colors.ENDC}")
        bm = Benchmark(bmp.stem, bmp)
        benchmarks[bmp.stem] = bm
    return benchmarks


def parse_benchmark_output(output_string):
    lines = output_string.splitlines()
    for i, line in enumerate(lines):
        if "elapsed" in line:
            return float(lines[i + 1])
    return -1e6


def run_benchmark(benchmark, iterations, warmup=1, quiet=False):
    timings = []
    print(80*"-")
    for i in range(1 - warmup, iterations + 1):
        if not quiet:
            sys.stdout.write(f"\rbenchmark: {benchmark.name:<30}{Colors.WARNING}iteration [{i}/{iterations}]{Colors.ENDC}")
            sys.stdout.flush()
        try:
            result = subprocess.run([interpreter_path, benchmark.path], capture_output=True, text=True)
        except subprocess.CalledProcessError as e:
            print(
                f"\n{Colors.FAIL}Unexpected interpreter failure\nerror code: {e.returncode}\nerror output: {e.stderr}")
            exit()
        timings.append(parse_benchmark_output(result.stdout))
    if not quiet:
        sys.stdout.write(f"\rbenchmark: {benchmark.name:<30}{Colors.OKBLUE}completed [{i}/{iterations}]{Colors.ENDC}\n")
        sys.stdout.flush()
    return timings


def print_benchmark_stats(timings, discard=1):
    bm_mean = fmean(timings[1:])
    bm_stdev = stdev(timings[1:])
    bm_stdevmean = bm_stdev / bm_mean
    color_prefix = Colors.FAIL if bm_stdevmean >= 0.1 else ""
    color_suffix = Colors.ENDC if bm_stdevmean >= 0.1 else ""
    print(f"{color_prefix}mean: {bm_mean:.2f} stdev: {bm_stdev:.2f} ({bm_stdevmean:.2f} means){color_suffix}")

def get_absolute_path(path: Path):
    if path.is_absolute():
        absolute_path = path
    else:
        absolute_path = Path.cwd().joinpath(path).resolve()
    return absolute_path


benchmarks = read_benchmarks()
choice_list = list(benchmarks.keys())
choice_list.sort()
parser = argparse.ArgumentParser(prog="benchmark", description="LoX benchmark runner")
parser.add_argument("-v", "--verbose",
                    action='store_true',
                    help="indicates process and gives detailed output",
                    dest="verbose",
                    default=False)
parser.add_argument("-b", "--benchmark", nargs='+',
                    choices=choice_list,
                    type=str,
                    help="benchmarks to run",
                    dest="bms",
                    default=choice_list)
parser.add_argument("-c", "--comment",
                    type=str,
                    help="benchmark comment added to info header",
                    dest="comment",
                    default="")
parser.add_argument("-e", "--executable",
                    type=str,
                    help="path to Lox interpeter",
                    dest="interpreter_path",
                    default=None)
parser.add_argument("-i", "--iterations", type=int,
                    help="iterations per benchmark",
                    dest="iterations",
                    default=5)
parser.add_argument("-o", "--output",
                    type=str,
                    nargs='?',
                    help="save output to file",
                    dest="output",
                    const='',
                    default=None)
parser.add_argument("-q", "-s", "--quiet", "--silent",
                    action='store_true',
                    help="only prints benchmark info and csv output to terminal, (same format as output files)",
                    dest="quiet",
                    default=False)

args = parser.parse_args()
verbose = args.verbose
bms = args.bms
interpreter_path = args.interpreter_path
output = args.output
iterations = args.iterations
bm_time = localtime()
quiet = args.quiet

if interpreter_path is None:
    interpreter_path = Path(__file__).parent.parent.joinpath(CMAKE_DIR).joinpath("cloxpp")
else:
    interpreter_path = get_absolute_path(interpreter_path)
# verify interpreter path is valid
if not (interpreter_path.exists() and os.access(interpreter_path, os.X_OK)):
    print(f"{Colors.WARNING}No lox interpreter found at the specified path: '{interpreter_path}'{Colors.ENDC}")
    exit()

if output is not None:
    if output == '':
        benchmark_path = Path(__file__).parent
        output_suffix = f"{strftime('%Y%m%d_%H%M%S', bm_time)}.txt"
        output_path = benchmark_path.joinpath(f"lox_benchmark_{output_suffix}")
        print(output_path)
    else:
        output_path = get_absolute_path(Path(output))
        if output_path.is_dir():
            output_suffix = f"{strftime('%Y%m%d_%H%M%S', bm_time)}.txt"
            output_path = output_path.joinpath(f"lox_benchmark_{output_suffix}")
        elif output_path.is_file():
            print(
                f"{Colors.WARNING}Warning a file with the same name '{output.name}' already exists in output path {output.parent}' and"
                f"will be overwritten after all benchmarks are finished (cancel this process to prevent this from "
                f"happening){Colors.ENDC}")


# print benchmark info
info_string = f"Lox interpreter benchmark {strftime('%Y-%m-%d %H:%M:%S', bm_time)}" \
              f"\ninterpreter: '{interpreter_path}'" \
              f"\nOS: '{platform.system()}'" \
              f"\nprocessor: '{platform.processor()}'" \
              f"\niterations: {iterations}" \
              f"\ncomment:{args.comment}"
print(info_string)

csv_output = 80*"-" + "\n"
for bm in bms:
    benchmark = benchmarks[bm]
    timings = run_benchmark(benchmark, iterations, quiet=quiet)
    #  stats from timings
    timing_string = ','.join(map(str, timings))
    csv_output += f"{bm},{timing_string}\n"
    if not quiet:
        print_benchmark_stats(timings)
    if verbose:
        timing_string = ','.join(map(str, timings))
        print(timing_string)
print(csv_output)

if output is not None:
    with open(output, 'w+') as f:
        f.write(info_string)
    with open(output, 'a') as f:
        f.write(csv_output)
    if verbose:
        print(f"successfully written benchmark data to '{output_path}'")



