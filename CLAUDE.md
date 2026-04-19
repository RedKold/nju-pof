# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a C-- language compiler front-end built with **Flex (lexical analyzer)** and **Bison (syntax parser)**. This is **Lab 2: Semantic Analysis**, which extends the parser from Lab 1 with:

- Symbol table management
- Type checking
- Semantic error detection (17 error types)
- Optional extensions (function declarations, nested scopes, structural type equivalence)

The project parses C-- source files (`.cmm` extension), builds an abstract syntax tree (AST), performs semantic analysis, and reports semantic errors.

## Lab 2: Semantic Analysis Details

### Current Status
- Lab 1 (lexical and syntax analysis) is already implemented
- The parser builds an AST and prints it on successful parsing
- Now we need to add semantic analysis

### Semantic Errors to Detect (17 Types)

1. **Error 1**: Variable used without definition
2. **Error 2**: Function called without definition  
3. **Error 3**: Variable redefinition or variable-struct name conflict
4. **Error 4**: Function redefinition
5. **Error 5**: Type mismatch in assignment
6. **Error 6**: Non-lvalue on left side of assignment
7. **Error 7**: Operand type mismatch
8. **Error 8**: Return type mismatch
9. **Error 9**: Function argument mismatch
10. **Error 10**: Array indexing on non-array
11. **Error 11**: Function call on non-function
12. **Error 12**: Non-integer array index
13. **Error 13**: Dot operator on non-struct
14. **Error 14**: Accessing undefined struct field
15. **Error 15**: Struct field redefinition or field initialization
16. **Error 16**: Struct name redefinition or struct-variable name conflict
17. **Error 17**: Using undefined struct

### Optional Requirements
- **3.1**: Function declarations (Errors 18-19)
- **3.2**: Nested scopes
- **3.3**: Structural type equivalence

### Error Output Format
```
Error type [error_type] at Line [line_number]: [description].
```

## Build and Run Instructions

### Prerequisites
- GCC compiler
- Flex (Fast Lexical Analyzer Generator)
- Bison (GNU Parser Generator)

### Build Process
The project uses a Makefile for automated building. From the `/Code` directory:

```bash
cd /home/saaya/pof/lab1/Code
make
```

This will:
1. Generate the lexical analyzer from `lexical.l` (produces `lex.yy.c`)
2. Generate the syntax parser from `syntax.y` (produces `syntax.tab.c` and `syntax.tab.h`)
3. Compile all C files into object files
4. Link everything together to create the final `parser` executable

### Run the Parser
To parse a C-- source file:

```bash
./parser <path_to_cmm_file>
```

### Test Target
The Makefile includes a `test` target that runs the parser on the default test file:

```bash
make test
```

This runs: `./parser ../Test/test4.cmm`

### Clean Build
To clean up generated files:

```bash
make clean
```

This removes:
- `parser` executable
- Generated parser/lexer files (`lex.yy.c`, `syntax.tab.c`, `syntax.tab.h`)
- Object files (`*.o`)
- Debug files (`*.d`)
- Bison debug output (`syntax.output`)

## Project Structure

### Key Files (from Lab 1)
- `/Code/lexical.l`: Flex lexical analyzer specification (tokenizer)
- `/Code/syntax.y`: Bison syntax parser specification (grammar rules + AST construction)
- `/Code/main.c`: Entry point that invokes the parser
- `/Code/Makefile`: Build automation script
- `/Test/`: Directory containing test files with `.cmm` extension
- `/Manual/`: Lab documentation including Project_2.pdf

### Files to Create for Lab 2
- `/Code/semantics.h`: Header file for semantic analysis
- `/Code/semantics.c`: Symbol table and type checking implementation

### Output Files
- `parser`: Compiled parser executable
- `syntax.output`: Bison debugging output (shows grammar states and conflicts)

## Language Features Supported

The parser supports a subset of C-like language features including:

### Data Types
- Integer (`int`)
- Float (`float`)
- Structs

### Variables and Arrays
- Variable declarations
- Array declarations (up to 2D)
- Struct definitions

### Functions
- Function definitions (and declarations with Requirement 3.1)
- Function parameters
- Function bodies with compound statements

### Control Flow
- If statements (with optional else)
- While loops
- Return statements

### Expressions
- Arithmetic operations: `+`, `-`, `*`, `/`
- Relational operations: `>`, `<`, `>=`, `<=`, `==`, `!=`
- Logical operations: `&&`, `||`, `!`
- Assignment: `=`
- Array indexing: `[]`
- Struct member access: `.`
- Function calls

### Literals
- Decimal integers
- Octal integers (starting with 0)
- Hexadecimal integers (starting with 0x)
- Floating-point numbers

## Semantic Analysis Implementation Notes

### Default Language Assumptions
1. `int` and `float` are incompatible for assignment and operations
2. Only `int` for logical operations and conditions; both for arithmetic
3. Functions defined exactly once (no declarations by default)
4. All variables have global scope (no nested scopes by default)
5. Struct type equivalence uses name equivalence
6. No nested functions
7. Struct fields and variables have unique names across structs

### Symbol Table Recommendations
- **Data Structure**: Hash table (use P.J. Weinberger hash function)
- **For Scopes**: Stack of tables (functional style) or depth links (imperative style)
- **Operations**: Insert, Lookup, Delete (for scopes)

### Type Representation
```c
typedef struct Type_ *Type;
typedef struct FieldList_ *FieldList;

struct Type_ {
    enum { BASIC, ARRAY, STRUCTURE } kind;
    union {
        int basic;  // int or float
        struct { Type elem; int size; } array;
        FieldList structure;
    } u;
};

struct FieldList_ {
    char* name;
    Type type;
    FieldList tail;
};
```

### Implementation Approach
1. Keep Bison for AST construction
2. Add separate semantic analysis pass that traverses the AST
3. First pass: Collect declarations and build symbol table
4. Second pass: Check types and expressions
5. Collect all errors before reporting (don't stop at first error)

## Error Handling

The parser provides error messages for:
- **Type A**: Lexical errors (invalid characters)
- **Type B**: Syntax errors (invalid grammar constructs)
- **Type 1-19**: Semantic errors (lab 2)

Semantic errors are reported with line numbers and error descriptions in the specified format.

## AST Construction

The parser builds an abstract syntax tree (AST) using `TreeNode` structures defined in `syntax.y`. Each node contains:
- Node type (e.g., `NODE_PROGRAM`, `NODE_EXPR`, `NODE_STMT`)
- Node name
- Line number
- Integer/float values (for literals)
- Child and sibling pointers (for tree structure)

The AST is printed to stdout if parsing completes successfully without errors (Lab 1 behavior). For Lab 2, we'll add semantic analysis after AST construction.
