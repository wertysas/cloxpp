#!/usr/bin/env python3
"""
Author: Johan Ericsson
"""
from enum import Enum
import subprocess
import os
import argparse
import json
from time import localtime, strftime

TIME_OUT = 2    # maximum running time in secs per test case

class Colours:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'


repo_path = os.path.dirname(os.path.abspath(__file__))

with open("sym_tsp.json") as fp:
    j = json.load(fp)

parser = argparse.ArgumentParser(prog="benchmark", description="LoX benchmark runner")
parser.add_argument("-t", "--test-case",
                    choices=list(j["problems"].keys()) + ["all"],
                    type=str,
                    help="Which testcase from TSPLIB to run",
                    dest="test_case",
                    default="all"
                    )
parser.add_argument("-e", "--executable",
                    type=str,
                    help="path to executable",
                    dest="prog",
                    default="/Users/johan/code/KTH/TSP2D/cmake-build-debug/TSP2D"
                    )

args = parser.parse_args()
test_case = args.test_case
binary_path = args.prog


# if test_case != "all":
#     try:
#         test_path = j["problems"][test_case]
#     except KeyError:
#         print("test case input (-t --t) wrong see help for more info, INTERNAL ERROR!?")
#
# elif test_case == "all":
#     print("SYMMETRIC TSP TEST " + strftime("%Y-%m-%d %H:%M:%S", localtime()))
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
#         # for i in range(N):
#         #     for j in range(N):
#         #         assert
#         # print(result.stdout)