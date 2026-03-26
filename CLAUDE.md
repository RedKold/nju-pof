```
# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.
```

## Project Overview

This is a C-- language parser and compiler front-end built with **Flex (lexical analyzer)** and **Bison (syntax parser)**. The project implements a complete syntax analyzer that parses C-- source files (`.cmm` extension) and generates an abstract syntax tree (AST) for further processing.

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

### Key Files
- `/Code/lexical.l`: Flex lexical analyzer specification (tokenizer)
- `/Code/syntax.y`: Bison syntax parser specification (grammar rules + AST construction)
- `/Code/main.c`: Entry point that invokes the parser
- `/Code/Makefile`: Build automation script
- `/Test/`: Directory containing test files with `.cmm` extension

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
- Function declarations
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

## Error Handling

The parser provides error messages for:
- Lexical errors (invalid characters)
- Syntax errors (invalid grammar constructs)

Errors are reported with line numbers and error types (Type A for lexical, Type B for syntax).

## AST Construction

The parser builds an abstract syntax tree (AST) using `TreeNode` structures defined in `syntax.y`. Each node contains:
- Node type (e.g., `NODE_PROGRAM`, `NODE_EXPR`, `NODE_STMT`)
- Node name
- Line number
- Integer/float values (for literals)
- Child and sibling pointers (for tree structure)

The AST is printed to stdout if parsing completes successfully without errors.
