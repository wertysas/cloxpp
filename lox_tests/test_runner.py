import argparse
from pathlib import Path

parser = argparse.ArgumentParser(
    prog="lox_tester",
    description="Lox interpreter test runner",
    epilog="Thanks for using lox_tester! Based on Robert Nystrom's testing code"
           " from the book Crafting Interpreters: https://craftinginterpreters.com/"
           " The Lox Test Runner is part of the cloxpp Lox interpreter project found at https://github.com/wertysas/cloxpp."
)
parser.add_argument("path", help="Path to lox interpreter")
parser.add_argument("test", default="all", help="Test to run")

args = parser.parse_args()
target_executable = Path(args.path)

if not target_executable.exists():
    print("Path to lox interpreter is invalid")
    raise SystemExit(1)
print("Identified lox interpreter at", target_executable.resolve())
