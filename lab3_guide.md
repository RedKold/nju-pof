# Guide: Lab 3 - Intermediate Code Generation

## Overview
Your task is to extend the C-- compiler to generate intermediate code (three-address code) from the AST after semantic analysis.

## Current Project Status
- ✅ Lab 1: Lexical & Syntax Analysis (complete)
- ✅ Lab 2: Semantic Analysis (complete) 
- 🔄 Lab 3: Intermediate Code Generation (in progress - you're here)

## Key Deliverables

### 1. New Files to Create
   - `translate.h` - Header file for intermediate code generation
   - `translate.c` - Implementation of translation functions

### 2. Files to Modify
   - `main.c` - Add intermediate code generation pass
   - `Makefile` - Add new source files to build system

## Intermediate Code Specification (From Chapter 4)

Your intermediate code must follow this format:

| Instruction | Description |
|-------------|-------------|
| `LABEL x:` | Define label `x` |
| `FUNCTION f:` | Define function `f` |
| `x := y` | Assign `y` to `x` |
| `x := y + z` | Arithmetic operations (+, -, *, /) |
| `x := &y` | Take address of `y` |
| `x := *y` | Dereference pointer `y` |
| `*x := y` | Write to address in `x` |
| `GOTO x` | Unconditional jump |
| `IF x [relop] y GOTO z` | Conditional jump |
| `RETURN x` | Return value from function |
| `DEC x [size]` | Allocate memory (for arrays/structs) |
| `ARG x` | Pass argument to function |
| `x := CALL f` | Call function and store result |
| `PARAM x` | Declare function parameter |
| `READ x` | Read integer from console |
| `WRITE x` | Write integer to console |

## Simplifying Assumptions (Basic Requirements)
1. No comments, octal/hex integers, floats
2. No structs or multi-dimensional arrays
3. Function params are only simple variables
4. No global variables, all variables unique
5. Functions don't return structs/arrays
6. Functions defined once (no declarations)
7. Input has no errors

## Implementation Steps

### Step 1: Design Intermediate Code Data Structure
Create structures to represent:
- Quadruples (op, arg1, arg2, result)
- Lists of intermediate code instructions
- Symbol table extensions (for variable offsets, etc.)

### Step 2: Implement Helper Functions
- `new_temp()` - Generate temporary variables (t1, t2, ...)
- `new_label()` - Generate labels (label1, label2, ...)
- `gen_code()` - Add instruction to list
- `print_ir()` - Output intermediate code to file

### Step 3: Implement Translation Functions
Traverse the AST and generate code for each node type:
```c
void translate_Program(TreeNode* node);
void translate_FuncDef(TreeNode* node);
void translate_Stmt(TreeNode* node);
void translate_Exp(TreeNode* node, char* place);
void translate_Cond(TreeNode* node, char* label_true, char* label_false);
void translate_Args(TreeNode* node, char** arg_list);
```

### Step 4: Integrate into Compiler
Modify `main.c`:
1. After semantic analysis passes, call translation functions
2. Output intermediate code to specified file

### Step 5: Handle Special Functions
- `read()` → `READ x` instruction
- `write(x)` → `WRITE x` instruction

## Key Translation Patterns (From Textbook)

### Expressions:
- `INT` → `place := #value`
- `ID` → `place := variable`
- `E1 + E2` → translate E1 to t1, E2 to t2, then `place := t1 + t2`
- `-E` → translate E to t1, then `place := #0 - t1`

### Control Flow:
- `if (E) S` → translate condition E, conditionally jump to S code
- `if (E) S1 else S2` → two labels for true/false paths
- `while (E) S` → loop with condition check at start

## How to Test
1. Create test input files (`.cmm`)
2. Run: `./parser input.cmm output.ir`
3. Verify output matches expected intermediate code format

## Optional Enhancements
- **4.1**: Support structs (and struct params)
- **4.2**: Support arrays (including multi-dimensional)

## Tips
- Start with simple expressions and assignments
- Add control flow step by step
- Test each feature independently before combining

## Reference Documents
- Chapter 4 of your textbook (provided text)
- Look at existing AST structures in `syntax.y`
- Use the semantic analysis as a guide for AST traversal
