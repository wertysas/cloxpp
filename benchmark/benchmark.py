#!/usr/bin/env python3
"""
Author: Johan Ericsson
"""
import sys
from enum import Enum
from dataclasses import dataclass
import subprocess
import os
import pathlib
from pathlib import Path
import argparse
from time import localtime, strftime


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


def parse_bechmark_output(output_string):
    lines = output_string.splitlines()
    for i, line in enumerate(lines):
        if "elapsed" in line:
            return float(lines[i+1])
    return -1e6


def run_benchmark(benchmark, iterations):
    print(80*"-")
    print(f"benchmark: {benchmark.name}")
    timings = []
    for i in range(iterations):
        sys.stdout.write(f"\r{Colors.WARNING}iteration: {i+1}/{iterations}{Colors.ENDC}")
        sys.stdout.flush()
        try:
            result = subprocess.run([interpreter_path, benchmark.path], capture_output=True, text=True)
        except subprocess.CalledProcessError as e:
            print(f"{Colors.FAIL}Unexpected interpreter failure\nerror code: {e.returncode}\nerror output: {e.stderr}")
        timings.append(parse_bechmark_output(result.stdout))
    sys.stdout.write("\r")
    sys.stdout.flush()
    print(*timings, sep=',')


benchmarks = read_benchmarks()
parser = argparse.ArgumentParser(prog="benchmark", description="LoX benchmark runner")
parser.add_argument("-v", "--verbose",
                    action='store_true',
                    help="verbose (debug) mode",
                    dest="verbose",
                    default=False)
parser.add_argument("-b", "--benchmark",
                    choices=list(benchmarks.keys()) + ["all"],
                    type=str,
                    help="benchmark to run",
                    dest="bm",
                    default="all")
file_path = Path(__file__).parent
parser.add_argument("-e", "--executable",
                    type=str,
                    help="path to Lox interpeter",
                    dest="interpreter_path",
                    default=None)
parser.add_argument("-i", "--iterations", type=int, nargs='+',
                    help="iterations per benchmark and discarded iterations at beginning"
                         "i.e. '-i 5 1' yields 5 iterations measures and one warm up iteration",
                    dest="iterations",
                    default=[5, 1])
args = parser.parse_args()
verbose = args.verbose
test_case = args.bm
interpreter_path = args.interpreter_path


# verify interpreter path
if interpreter_path is None:
    interpreter_path = Path(__file__).parent.parent.joinpath(CMAKE_DIR).joinpath("cloxpp")
else:
    interpreter_path = Path(interpreter_path)
if not (interpreter_path.exists() and os.access(interpreter_path, os.X_OK)):
    print(f"{Colors.WARNING}No lox interpreter found at the specified path: '{interpreter_path}'{Colors.ENDC}")
    exit()

if verbose:
    print(f"Lox interpreter identified: '{interpreter_path}'")
    print(80*'-')
# verify iterations is passed correctly
if len(args.iterations) > 2:
    parser.error("Max two arguments can be provided for the number of iterations")
iterations = (args.iterations, 1) if len(args.iterations) == 1 else args.iterations


print(f"Lox interpreter benchmark {strftime('%Y-%m-%d %H:%M:%S', localtime())}")
print(f"interpreter executable: '{interpreter_path}'")
print(f"iterations/benchmark: {iterations[0]}")
print(f"warmup iterations/benchmark: {iterations[1]}")

if test_case != "all":
    try:
        benchmark = benchmarks[test_case]
        run_benchmark(benchmark, iterations[0]+iterations[1])

    except KeyError:
        print(f"{Colors.WARNING}No benchmark named: '{test_case}', see help (-h) for more info.{Colors.ENDC}")
        exit()



# elif test_case == "all":
#     print("Lox interpreter benchmark" + strftime("%Y-%m-%d %H:%M:%S", localtime()))
#     print(f"executable: {}")
#     for name in j["problem_names"]:
#         print(20 * "-" + " TESTCASE: " + name + 20 * "-")
#         try:
#             test_path = j["problems"][name]["points"]
#             optimal_path = j["problems"][name]["optimal"]
#         except KeyError:
#             print("test case input (-t --t) wrong see help for more info, INTERNAL ERROR!?")
#         t = TestCase()
#         test_path = os.path.join(repo_path, test_path)
#         optimal_path = os.path.join(repo_path, optimal_path)
#         t.read_test_case(name, test_path, optimal_path)
#         with open("testcase.in", "w+") as fin:
#             fin.write(t.format_points())
#         fin = open("testcase.in", "r")
#         try:
#             result = subprocess.run([binary_path], stdin=fin, capture_output=True, text=True, timeout=TIME_OUT)
#             fin.close()
#             t.parse_result(result.stdout)
#
#         except subprocess.TimeoutExpired:
#             print(Colours.WARNING + "TimeoutError runtime longer than" + str(TIME_OUT)+"s" + Colours.ENDC)
      # for i in range(N):
      #     for j in range(N):
      #         assert
      # print(result.stdout)
