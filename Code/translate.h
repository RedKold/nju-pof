#ifndef TRANSLATE_H
#define TRANSLATE_H

#include "tree/tree.h"
#include "semantics.h"
#include <stdio.h>

// ==================== Intermediate Code Data Structures ====================

// Intermediate code instruction types
typedef enum {
    IR_LABEL,
    IR_FUNCTION,
    IR_ASSIGN,
    IR_BINOP,    // Binary operations: +, -, *, /
    IR_UNIOP,    // Unary operations: -
    IR_ADDR,     // x := &y
    IR_DEREF_L,  // x := *y
    IR_DEREF_R,  // *x := y
    IR_GOTO,
    IR_IF_GOTO,
    IR_RETURN,
    IR_DEC,
    IR_ARG,
    IR_CALL,
    IR_PARAM,
    IR_READ,
    IR_WRITE
} IRType;

// Binary operator types
typedef enum {
    OP_PLUS,
    OP_MINUS,
    OP_MUL,
    OP_DIV
} BinOp;

// Relational operator types
typedef enum {
    RELOP_LT,
    RELOP_LE,
    RELOP_GT,
    RELOP_GE,
    RELOP_EQ,
    RELOP_NE
} RelOp;

// Intermediate code instruction structure
typedef struct IRInstruction_* IRInstruction;
typedef struct IRInstruction_ {
    IRType type;
    union {
        // IR_LABEL: label
        struct { char label[32]; } label;

        // IR_FUNCTION: function name
        struct { char func[32]; } func;

        // IR_ASSIGN: x := y
        struct { char left[32]; char right[32]; } assign;

        // IR_BINOP: x := y op z
        struct { char result[32]; char arg1[32]; BinOp op; char arg2[32]; } binop;

        // IR_UNIOP: x := op y
        struct { char result[32]; char arg[32]; } uniop;

        // IR_ADDR: x := &y
        struct { char left[32]; char right[32]; } addr;

        // IR_DEREF_L: x := *y
        struct { char left[32]; char right[32]; } deref_l;

        // IR_DEREF_R: *x := y
        struct { char left[32]; char right[32]; } deref_r;

        // IR_GOTO: GOTO label
        struct { char label[32]; } goto_;

        // IR_IF_GOTO: IF x relop y GOTO label
        struct { char arg1[32]; RelOp op; char arg2[32]; char label[32]; } if_goto;

        // IR_RETURN: RETURN x
        struct { char val[32]; } return_;

        // IR_DEC: DEC x size
        struct { char var[32]; int size; } dec;

        // IR_ARG: ARG x
        struct { char arg[32]; } arg;

        // IR_CALL: x := CALL f
        struct { char result[32]; char func[32]; } call;

        // IR_PARAM: PARAM x
        struct { char param[32]; } param;

        // IR_READ: READ x
        struct { char var[32]; } read;

        // IR_WRITE: WRITE x
        struct { char var[32]; } write;
    } u;
    IRInstruction prev;
    IRInstruction next;
} IRInstruction_;

// Intermediate code list structure
typedef struct IRList_ {
    IRInstruction head;
    IRInstruction tail;
    int temp_count;
    int label_count;
} IRList_;

// ==================== Function Declarations ====================

// Initialize the intermediate code generation system
void initTranslate();

// Clean up resources
void cleanupTranslate();

// Generate a new temporary variable name (t1, t2, ...)
char* new_temp();

// Generate a new label name (label1, label2, ...)
char* new_label();

// ==================== Code Generation Functions ====================

// Main translation function - traverses the AST and generates intermediate code
void translateProgram(TreeNode* root, const char* outputFilename);

// Translate a function definition
void translateFuncDef(TreeNode* node);

// Translate a statement
void translateStmt(TreeNode* node);

// Translate an expression and put result in 'place'
void translateExp(TreeNode* node, char* place);

// Translate a condition expression, jumping to label_true or label_false
void translateCond(TreeNode* node, char* label_true, char* label_false);

// Translate function arguments
void translateArgs(TreeNode* node, char** arg_list, int* arg_count);

// ==================== Helper Functions ====================

// Add an instruction to the IR list
void addIR(IRInstruction inst);

// Create a new IR instruction (to be filled by caller)
IRInstruction createIR(IRType type);

// Print the generated intermediate code to file
void printIR(FILE* out);

#endif // TRANSLATE_H
