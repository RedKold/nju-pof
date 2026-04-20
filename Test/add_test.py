#!/usr/bin/env python3
"""
Helper script to create new test cases.
Usage: ./add_test.py test_name "Test description"
"""

import os
import sys
from pathlib import Path

TEST_DIR = Path(__file__).parent / "semantic_tests"

TEMPLATE_CMM = """/*
 * Test: {test_name}
 * Description: {description}
 */

int main()
{{
    // Your test code here
}}
"""


def main():
    if len(sys.argv) < 3:
        print("Usage: ./add_test.py <test_name> \"<description>\"")
        print("Example: ./add_test.py error01_undefined_var \"Undefined variable test\"")
        return 1

    test_name = sys.argv[1]
    description = " ".join(sys.argv[2:])

    cmm_file = TEST_DIR / f"{test_name}.cmm"
    expected_file = TEST_DIR / f"{test_name}.expected"

    if cmm_file.exists():
        print(f"Error: Test {test_name} already exists!")
        return 1

    with open(cmm_file, "w") as f:
        f.write(TEMPLATE_CMM.format(test_name=test_name, description=description))

    # Create empty expected file
    expected_file.touch()

    print(f"Created test:")
    print(f"  {cmm_file}")
    print(f"  {expected_file}")
    print()
    print("Now edit the .cmm file with your test code,")
    print("and the .expected file with the expected output.")

    return 0


if __name__ == "__main__":
    sys.exit(main())
