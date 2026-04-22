#!/usr/bin/env python3
"""
Test Framework for C-- Semantic Analysis
Run this script to test your semantic analyzer.
"""

import os
import sys
import subprocess
import difflib
from pathlib import Path

# Configuration
SCRIPT_DIR = Path(__file__).parent.resolve()
PROJECT_ROOT = SCRIPT_DIR.parent
CODE_DIR = PROJECT_ROOT / "Code"
PARSER = CODE_DIR / "parser"
TEST_DIR = SCRIPT_DIR / "semantic_tests"


def color_text(text, color):
    """Add ANSI color codes to text."""
    colors = {
        "red": "\033[91m",
        "green": "\033[92m",
        "yellow": "\033[93m",
        "blue": "\033[94m",
        "bold": "\033[1m",
        "reset": "\033[0m"
    }
    return f"{colors.get(color, '')}{text}{colors['reset']}"


def run_test(test_file):
    """Run a single test file and return (success, actual_output, expected_output)."""
    test_name = test_file.stem
    cmm_file = test_file
    expected_file = test_file.with_suffix(".expected")

    # Read expected output
    expected = []
    if expected_file.exists():
        with open(expected_file, "r") as f:
            expected = [line.rstrip("\n") for line in f]

    # Run the parser
    try:
        result = subprocess.run(
            [str(PARSER), str(cmm_file)],
            capture_output=True,
            text=True,
            timeout=10
        )
        all_actual = [line.rstrip("\n") for line in result.stdout.splitlines()]
        # Only keep lines that start with "Error" for comparison
        actual = [line for line in all_actual if line.startswith("Error")]
    except subprocess.TimeoutExpired:
        return False, ["TIMEOUT"], expected, []
    except Exception as e:
        return False, [f"ERROR: {e}"], expected, []

    # Compare
    success = actual == expected

    return success, actual, expected, all_actual


def print_diff(actual, expected):
    """Print a unified diff between actual and expected output."""
    diff = difflib.unified_diff(
        expected, actual,
        fromfile="Expected",
        tofile="Actual",
        lineterm=""
    )
    for line in diff:
        if line.startswith("+"):
            print(color_text(line, "green"))
        elif line.startswith("-"):
            print(color_text(line, "red"))
        elif line.startswith("@"):
            print(color_text(line, "blue"))
        else:
            print(line)


def main():
    print(color_text("=" * 60, "bold"))
    print(color_text("C-- Semantic Analysis Test Runner", "bold"))
    print(color_text("=" * 60, "bold"))
    print()

    # Check if parser exists
    if not PARSER.exists():
        print(color_text(f"ERROR: Parser not found at {PARSER}", "red"))
        print("Please build the parser first with 'make' in the Code directory.")
        return 1

    # Find all test files
    test_files = sorted(TEST_DIR.glob("*.cmm"))

    if not test_files:
        print(color_text("No test files found in semantic_tests/", "yellow"))
        print("Create .cmm files in Test/semantic_tests/")
        print("Create corresponding .expected files with expected output.")
        return 0

    passed = 0
    failed = 0

    # Run each test
    for test_file in test_files:
        test_name = test_file.stem
        print(f"{color_text('Test:', 'bold')} {test_name}... ", end="")

        success, actual, expected, all_actual = run_test(test_file)

        if success:
            print(color_text("PASS", "green"))
            passed += 1
        else:
            print(color_text("FAIL", "red"))
            failed += 1
            print()
            print(color_text("  Expected:", "yellow"))
            for line in expected:
                print(f"    {line}")
            print(color_text("  Actual Errors:", "yellow"))
            for line in actual:
                print(f"    {line}")
            print()
            print(color_text("  Diff:", "yellow"))
            print_diff(actual, expected)
            print()

    # Summary
    print(color_text("=" * 60, "bold"))
    total = passed + failed
    print(color_text(f"Summary: {passed}/{total} passed", "bold"))
    if failed > 0:
        print(color_text(f"         {failed} failed", "red"))
    else:
        print(color_text("         All tests passed!", "green"))
    print(color_text("=" * 60, "bold"))

    return 0 if failed == 0 else 1


if __name__ == "__main__":
    sys.exit(main())
