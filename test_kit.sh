#!/bin/bash

# Test kit for C-- parser implementation
# Runs all tests in Resources/Tests directory and compares outputs with expected results

# Configuration
TEST_DIR="/home/saaya/pof/lab1/Resources/Tests"
INPUT_DIR="$TEST_DIR/inputs"
EXPECT_DIR="$TEST_DIR/expects"
PARSER="/home/saaya/pof/lab1/Code/parser"
TEMP_DIR="$TEST_DIR/tmp/parser_test_outputs"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Results tracking
PASS=0
FAIL=0
TOTAL=0

# Create temporary directory for test outputs
mkdir -p "$TEMP_DIR"

# Function to run a single test
run_test() {
    local test_name="$1"
    local input_file="$INPUT_DIR/$test_name.cmm"
    local expect_file="$EXPECT_DIR/$test_name.exp"
    local output_file="$TEMP_DIR/$test_name.out"

    TOTAL=$((TOTAL + 1))

    # Check if input and expected files exist
    if [ ! -f "$input_file" ]; then
        echo -e "${RED}[ERROR]${NC} Input file not found: $input_file"
        FAIL=$((FAIL + 1))
        return 1
    fi

    if [ ! -f "$expect_file" ]; then
        echo -e "${YELLOW}[SKIP]${NC} Expected output file not found: $expect_file"
        return 1
    fi

    # Run the parser on the test input
    "$PARSER" "$input_file" > "$output_file" 2>&1

    # Compare the output - only check error type and line number
    # For error messages, look for "Error type [A|B] at Line [number]"
    # For AST outputs, do a full comparison

    local test_passed=true

    # Check if the output contains any error messages
    if grep -q "Error type [AB] at Line [0-9]" "$expect_file"; then
        # Both files contain error messages - check if error type and line match
        local expected_error=$(grep "Error type [AB] at Line [0-9]" "$expect_file")
        local actual_error=$(grep "Error type [AB] at Line [0-9]" "$output_file")

        if [ -z "$actual_error" ]; then
            test_passed=false
        else
            # Extract error type and line number from both
            local expected_type=$(echo "$expected_error" | grep -o "Error type [AB]" | grep -o "[AB]")
            local expected_line=$(echo "$expected_error" | grep -o "at Line [0-9]*" | grep -o "[0-9]*")
            local actual_type=$(echo "$actual_error" | grep -o "Error type [AB]" | grep -o "[AB]")
            local actual_line=$(echo "$actual_error" | grep -o "at Line [0-9]*" | grep -o "[0-9]*")

            if [ "$expected_type" != "$actual_type" ] || [ "$expected_line" != "$actual_line" ]; then
                test_passed=false
            fi
        fi
    else
        # No error messages - do full AST comparison
        if ! diff -u "$expect_file" "$output_file" > /dev/null; then
            test_passed=false
        fi
    fi

    if [ "$test_passed" = true ]; then
        echo -e "${GREEN}[PASS]${NC} $test_name"
        PASS=$((PASS + 1))
    else
        echo -e "${RED}[FAIL]${NC} $test_name"
        FAIL=$((FAIL + 1))
        echo "    Output saved to: $output_file"
        echo "    Expected: $expect_file"
        echo "    Diff:"
        diff -u "$expect_file" "$output_file" | head -20
    fi
}

# Main test execution
echo "=============================================="
echo "C-- Parser Test Kit"
echo "=============================================="
echo "Parser: $PARSER"
echo "Test directory: $TEST_DIR"
echo "Temporary output directory: $TEMP_DIR"
echo "=============================================="
echo ""

# Run all tests
for input_file in "$INPUT_DIR"/*.cmm; do
    # Extract test name (without .cmm extension)
    test_name=$(basename "$input_file" .cmm)
    run_test "$test_name"
done

echo ""
echo "=============================================="
echo "Test Results Summary"
echo "=============================================="
echo "Total tests: $TOTAL"
echo -e "Passed: ${GREEN}$PASS${NC}"
echo -e "Failed: ${RED}$FAIL${NC}"
if [ $TOTAL -gt 0 ]; then
    local PASS_RATE=$((PASS * 100 / TOTAL))
    echo "Pass rate: $PASS_RATE%"
fi
echo "=============================================="

# Cleanup temporary directory
rm -rf "$TEMP_DIR"

# Exit with failure code if any tests failed
if [ $FAIL -gt 0 ]; then
    exit 1
else
    exit 0
fi
