# Semantic Analysis Test Kit

Quickly test your semantic analyzer with this test framework.

## Usage

### Run all tests
```bash
cd Code
make semantic-test
```

Or directly:
```bash
cd Test
python3 test_runner.py
```

### Add a new test
```bash
cd Test
./add_test.py error05_type_mismatch "Type mismatch in assignment"
```

This creates:
- `semantic_tests/error05_type_mismatch.cmm` - your test code
- `semantic_tests/error05_type_mismatch.expected` - expected output

Edit both files, then run the test!

### Test File Organization
```
Test/
├── test_runner.py          # Main test runner
├── add_test.py             # Helper to create new tests
├── semantic_tests/
│   ├── error01_undefined_var.cmm       # Test case
│   ├── error01_undefined_var.expected  # Expected output
│   └── ...
└── README_TESTKIT.md      # This file
```

## Expected Output Format

Match the error format from the lab guide:
```
Error type 1 at Line 4: Undefined variable "j".
Error type 5 at Line 6: Type mismatched for assignment.
```

## Tips

1. Start with simple tests first (Error 1, Error 3, etc.)
2. One test case per error type is a good starting point
3. You can have multiple errors in one test file
4. The order of error messages matters!
