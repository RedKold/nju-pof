# C-- Compiler Lab 2: Semantic Analysis

## Overview
This lab extends the C-- parser from Lab 1 with semantic analysis capabilities. You will implement a symbol table, type checking, and detect 17 different types of semantic errors.

## Key Concepts

### Semantic Analysis
Semantic analysis is the phase after syntax analysis that:
- Checks for context-sensitive errors
- Builds and manages a symbol table
- Performs type checking
- Prepares for intermediate code generation

### Attribute Grammar
- **Synthesized Attributes**: Pass information up the parse tree (bottom-up)
- **Inherited Attributes**: Pass information down or across the parse tree (top-down)

### Symbol Table
Stores information about identifiers (variables, functions, structs):
- Name
- Type
- Scope
- Other relevant properties

Data structure options:
- **Linear Linked List**: Simple, O(n) search
- **Balanced Binary Tree**: O(log n) search
- **Hash Table**: O(1) average search time (recommended)

### Type System
C-- has:
- Basic types: `int`, `float`
- Constructed types: arrays (up to 2D), structs
- Type equivalence: Name equivalence (default) or Structural equivalence (optional)

## Default Language Assumptions

1. **int** and **float** variables cannot be mutually assigned or operated on
2. Only `int` can be used for logical operations and in `if`/`while` conditions; both `int` and `float` can participate in arithmetic operations
3. Each function is defined exactly once (no function declarations, only definitions)
4. All variables (including function parameters) have global scope (no nested scopes)
5. Struct type equivalence uses name equivalence
6. Functions cannot be nested
7. Struct fields and variable names are unique across different structs

## Semantic Errors to Detect (17 Types)

### Basic Errors
1. **Error 1**: Variable used without definition
2. **Error 2**: Function called without definition  
3. **Error 3**: Variable redefinition (or variable name conflicts with struct name)
4. **Error 4**: Function redefinition
5. **Error 5**: Type mismatch in assignment
6. **Error 6**: Non-lvalue expression on left side of assignment
7. **Error 7**: Type mismatch between operands in an operation
8. **Error 8**: Return type doesn't match function's return type
9. **Error 9**: Number or types of arguments don't match function parameters
10. **Error 10**: Array indexing used on non-array type
11. **Error 11**: Function call operator `()` used on non-function type
12. **Error 12**: Non-integer used as array index
13. **Error 13**: Dot operator `.` used on non-struct type
14. **Error 14**: Accessing undefined field in struct
15. **Error 15**: Redefinition of field in struct (or field initialization in struct definition)
16. **Error 16**: Struct name redefinition (or struct name conflicts with variable name)
17. **Error 17**: Using undefined struct to define variable

### Optional Extended Errors (for Requirements 3.1-3.3)

**Requirement 3.1 (Function Declarations)**:
- **Error 18**: Function declared but not defined
- **Error 19**: Multiple declarations of function conflict (return type, parameters, or calling convention mismatch)
- Also need to add grammar support for function declarations

**Requirement 3.2 (Nested Scopes)**:
- Variables in inner blocks can shadow outer block variables
- Variables in different functions are in separate scopes
- Requires implementing scope management

**Requirement 3.3 (Structural Type Equivalence)**:
- Two structs are equivalent if they have the same field names and types in the same order
- Name equivalence is no longer used

## Error Output Format

```
Error type [error_type] at Line [line_number]: [description].
```

Example:
```
Error type 1 at Line 4: Undefined variable "j".
```

## Implementation Approach

### Option 1: Integrate with Bison
- Embed semantic actions directly in Bison rules (similar to SDT)
- Pros: Fewer files to manage
- Cons: More complex attribute flow, may need to use only synthesized attributes

### Option 2: Separate Semantic Analysis (Recommended)
- Bison only constructs the AST
- Traverse the AST in a separate pass for semantic analysis
- Pros: Clean separation, more flexible attribute handling
- Cons: Need to implement AST traversal

### Key Steps:
1. **Design Symbol Table**: Choose hash table with chaining or open addressing
2. **Design Type Representation**: Use linked structures for arrays and structs
3. **Implement Symbol Table Operations**: Insert, lookup, delete (for scopes)
4. **Traverse AST**: Visit declaration nodes to fill symbol table, visit expression nodes to check types
5. **Error Reporting**: Collect all errors and print in order (don't stop at first error)

## Symbol Table Design Recommendations

- Use a hash table with the P.J. Weinberger hash function (provided in the manual)
- For scope support (Requirement 3.2), choose either:
  - **Functional Style**: Stack of symbol tables (push on block entry, pop on exit)
  - **Imperative Style**: Single table with depth information, linked lists per scope level

## Type Representation

```c
typedef struct Type_ *Type;
typedef struct FieldList_ *FieldList;

struct Type_ {
    enum { BASIC, ARRAY, STRUCTURE } kind;
    union {
        // Basic type
        int basic;  // int or float
        // Array type
        struct { Type elem; int size; } array;
        // Struct type
        FieldList structure;
    } u;
};

struct FieldList_ {
    char* name;   // Field name
    Type type;    // Field type
    FieldList tail; // Next field
};
```

## Files to Modify/Create

### Existing Files (from Lab 1)
- `lexical.l`: May need minor updates
- `syntax.y`: May need to add productions for function declarations (if doing Requirement 3.1), and ensure AST has all necessary info
- `main.c`: Will need to call semantic analysis after parsing

### New Files (Recommended)
- `semantics.h`: Header file for semantic analysis
- `semantics.c`: Symbol table and type checking implementation
- `ast.h`: (Optional) Separate AST definitions if not already in `syntax.y`

## Build and Run

### Build
```bash
cd Code
make
```

### Run
```bash
./parser <input_file.cmm>
```

### Expected Output
- If no semantic errors: No output
- If semantic errors: Error messages in the specified format

## Examples

### Example 1 (Error 1)
**Input**:
```c
int main()
{
    int i = 0;
    j = i + 1;
}
```

**Output**:
```
Error type 1 at Line 4: Undefined variable "j".
```

### Example 5 (Error 5)
**Input**:
```c
int main()
{
    int i;
    i = 3.7;
}
```

**Output**:
```
Error type 5 at Line 4: Type mismatched for assignment.
```

See the Project_2.pdf for all 17 examples.

## Optional Requirements (Choose One or More)

1. **Requirement 3.1**: Function declarations
   - Allow functions to be declared before definition
   - Check consistency between declarations and definition
   - Add Errors 18 and 19

2. **Requirement 3.2**: Nested scopes
   - Variables have block-level scope
   - Inner scope can shadow outer scope
   - Different functions have separate scopes

3. **Requirement 3.3**: Structural type equivalence
   - Compare structs by their structure, not just name
   - Requires deep comparison of struct types

## Submission Requirements

1. Working Flex/Bison/C code that compiles without errors
2. PDF lab report (max 3 pages, min 11pt font)
   - What functions you implemented and how
   - How to compile your program
   - Highlight your unique/interesting features

## Tips

1. **Don't stop at first error**: Continue parsing and collect all errors
2. **Test incrementally**: Test each error type one by one
3. **Use AST**: The more information in your AST, the easier semantic analysis will be
4. **Hash function**: The P.J. Weinberger hash function is a good choice for symbol names
5. **Memory management**: Be careful with memory allocation for types and symbol table entries
