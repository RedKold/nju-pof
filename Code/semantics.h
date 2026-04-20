#ifndef SEMANTICS_H
#define SEMANTICS_H

#include "tree/tree.h"
#include <stdbool.h>

// ==================== Type System Definitions ====================

typedef struct Type_* Type;
typedef struct FieldList_* FieldList;

// Type kinds
typedef enum {
    TYPE_BASIC,
    TYPE_ARRAY,
    TYPE_STRUCTURE
} TypeKind;

// Basic types
typedef enum {
    BASIC_INT,
    BASIC_FLOAT
} BasicType;

struct Type_ {
    TypeKind kind;
    union {
        BasicType basic;  // for TYPE_BASIC
        struct {           // for TYPE_ARRAY
            Type elem;
            int size;
        } array;
        FieldList structure;  // for TYPE_STRUCTURE
    } u;
};

struct FieldList_ {
    char name[32];  // field name
    Type type;      // field type
    FieldList tail; // next field
};

// ==================== Symbol Table Definitions ====================

// Symbol kinds
typedef enum {
    SYMBOL_VARIABLE,
    SYMBOL_FUNCTION,
    SYMBOL_STRUCT
} SymbolKind;

// Function type information
typedef struct {
    Type returnType;
    int paramCount;
    FieldList params;  // parameter list
    bool defined;      // whether the function is defined (for Requirement 3.1)
} FunctionInfo;

// Symbol structure
typedef struct Symbol_ {
    char name[32];
    SymbolKind kind;
    union {
        Type variableType;   // for SYMBOL_VARIABLE
        FunctionInfo funcInfo; // for SYMBOL_FUNCTION
        Type structType;     // for SYMBOL_STRUCT
    } info;
    struct Symbol_* next;  // for hash table chaining
    int depth;              // for scope management (Requirement 3.2)
} Symbol;

// ==================== Semantic Analysis Functions ====================

// Initialize semantic analysis
void initSemantics();

// Clean up semantic analysis resources
void cleanupSemantics();

// Main semantic analysis function - traverses the AST and performs checks
void analyzeSemantics(TreeNode* root);

// ==================== Error Reporting Functions ====================

// Report semantic error
void semanticError(int errorType, int lineNumber, const char* description);

// Get total number of semantic errors
int getSemanticErrorCount();

#endif // SEMANTICS_H
