#include "semantics.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

// ==================== Symbol Table Configuration ====================

#define HASH_TABLE_SIZE 16384  // 2^14, as recommended in the manual

// ==================== Global Variables ====================

static Symbol* hashTable[HASH_TABLE_SIZE] = {NULL};
static int semanticErrorCount = 0;
static int currentScopeDepth = 0;  // for Requirement 3.2 (nested scopes)
static bool inFirstPass = false;     // true = collecting declarations, false = checking usage

// ==================== Forward Declarations ====================

static Type analyzeSpecifier(TreeNode* node);
static Type analyzeVarDec(TreeNode* node, Type type, bool isParam);
static FieldList analyzeDefListForFields(TreeNode* defListNode, int structLine);
static void analyzeExtDef(TreeNode* node);
static void analyzeDef(TreeNode* node);
static void analyzeStmt(TreeNode* node);
static Type analyzeExpr(TreeNode* node);
static void analyzeCompSt(TreeNode* node);
static void processVarDec(TreeNode* varDecNode, Type baseType, int lineNo);
static void traverseExtDefList(TreeNode* extDefList);

// ==================== Hash Function (P.J. Weinberger) ====================

static unsigned int hashpjw(const char* name) {
    unsigned int val = 0, i;
    for (; *name; ++name) {
        val = (val << 2) + *name;
        if ((i = val & 0x3fff0000)) {
            val = (val ^ (i >> 12)) & 0x3fff;
        }
    }
    return val % HASH_TABLE_SIZE;
}

// ==================== Symbol Table Operations ====================

// Insert a symbol into the hash table
static Symbol* insertSymbol(const char* name, SymbolKind kind) {
    unsigned int hash = hashpjw(name);

    Symbol* symbol = (Symbol*)malloc(sizeof(Symbol));
    if (!symbol) {
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }

    strncpy(symbol->name, name, 31);
    symbol->name[31] = '\0';
    symbol->kind = kind;
    symbol->depth = currentScopeDepth;
    symbol->next = hashTable[hash];
    hashTable[hash] = symbol;

    return symbol;
}

// ==================== Log Function ====================

static void Log(const char* func, const char* msg, ...) {
    va_list args;
    va_start(args, msg);
    printf("[%s] ", func);
    vprintf(msg, args);
    printf("\n");
    va_end(args);
}

// Lookup a symbol in the hash table
static Symbol* lookupSymbol(const char* name) {
    unsigned int hash = hashpjw(name);
    Log(__func__, "Finding name: %s", name);
    Symbol* symbol = hashTable[hash];

    // For Requirement 3.2: find the symbol with the highest depth (innermost scope)
    Symbol* found = NULL;
    int maxDepth = -1;

    while (symbol) {
        if (strcmp(symbol->name, name) == 0) {
            if (symbol->depth > maxDepth) {
                maxDepth = symbol->depth;
                found = symbol;
            }
        }
        symbol = symbol->next;
    }

    if(found){
        Log(__func__, "Found");
    }
    else{
        Log(__func__, "Not found");
    }

    return found;
}

// Remove all symbols at the current scope depth (for Requirement 3.2)
static void exitScope() {
    for (int i = 0; i < HASH_TABLE_SIZE; ++i) {
        Symbol** symbolPtr = &hashTable[i];
        while (*symbolPtr) {
            if ((*symbolPtr)->depth == currentScopeDepth) {
                Symbol* temp = *symbolPtr;
                *symbolPtr = (*symbolPtr)->next;
                free(temp);
            } else {
                symbolPtr = &(*symbolPtr)->next;
            }
        }
    }
    currentScopeDepth--;
}

// Enter a new scope (for Requirement 3.2)
// 每次进入新的作用域，都提升一个深度
static void enterScope() {
    currentScopeDepth++;
}

// ==================== Type Operations ====================

// Create a basic type (int or float)
static Type createBasicType(BasicType basic) {
    Type type = (Type)malloc(sizeof(struct Type_));
    if (!type) {
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }
    type->kind = TYPE_BASIC;
    type->u.basic = basic;
    return type;
}

// Create an array type
static Type createArrayType(Type elem, int size) {
    Type type = (Type)malloc(sizeof(struct Type_));
    if (!type) {
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }
    type->kind = TYPE_ARRAY;
    type->u.array.elem = elem;
    type->u.array.size = size;
    return type;
}

// Create a struct type
static Type createStructType(FieldList fields) {
    Type type = (Type)malloc(sizeof(struct Type_));
    if (!type) {
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }
    type->kind = TYPE_STRUCTURE;
    type->u.structure = fields;
    return type;
}

// Create a field list node
static FieldList createField(const char* name, Type type) {
    FieldList field = (FieldList)malloc(sizeof(struct FieldList_));
    if (!field) {
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }
    strncpy(field->name, name, 31);
    field->name[31] = '\0';
    field->type = type;
    field->tail = NULL;
    return field;
}

// Check two types for equivalence (name equivalence by default)
static bool typeEqual(Type t1, Type t2) {
    if (!t1 || !t2) return false;
    if (t1->kind != t2->kind) return false;

    switch (t1->kind) {
        case TYPE_BASIC:
            return t1->u.basic == t2->u.basic;
        case TYPE_ARRAY:
            // For arrays, same element type and same size
            return (t1->u.array.size == t2->u.array.size) &&
                   typeEqual(t1->u.array.elem, t2->u.array.elem);
        case TYPE_STRUCTURE:
            // For Requirement 3.3 (structural equivalence), you'd compare fields here
            // For now (name equivalence), this requires storing struct names
            // For simplicity, let's return false here (you need to implement this properly)
            return false;
        default:
            return false;
    }
}

// Free a type (helper function)
static void freeType(Type type) {
    if (!type) return;
    switch (type->kind) {
        case TYPE_ARRAY:
            freeType(type->u.array.elem);
            break;
        case TYPE_STRUCTURE: {
            FieldList field = type->u.structure;
            while (field) {
                FieldList next = field->tail;
                freeType(field->type);
                free(field);
                field = next;
            }
            break;
        }
        default:
            break;
    }
    free(type);
}

// ==================== Error Reporting ====================

void semanticError(int errorType, int lineNumber, const char* description) {
    printf("Error type %d at Line %d: %s.\n", errorType, lineNumber, description);
    semanticErrorCount++;
}

int getSemanticErrorCount() {
    return semanticErrorCount;
}

// ==================== AST Traversal Helper Functions ====================

// Get the identifier name from a node (usually a T_ID child)
static const char* getIdentifierName(TreeNode* node) {
    if (!node) return NULL;
    // If this is an ID node
    if (node->type == NODE_ID) {
        return node->name;
    }
    // Otherwise look for an ID child
    TreeNode* child = node->firstChild;
    while (child) {
        const char* name = getIdentifierName(child);
        if (name) return name;
        child = child->nextSibling;
    }
    return NULL;
}

// Check if a field name already exists in a field list
static bool fieldExists(FieldList fields, const char* name) {
    while (fields) {
        if (strcmp(fields->name, name) == 0) {
            return true;
        }
        fields = fields->tail;
    }
    return false;
}

// Analyze a DefList to extract struct fields
static FieldList analyzeDefListForFields(TreeNode* defListNode, int structLine) {
    FieldList head = NULL;
    FieldList tail = NULL;

    // Traverse defList properly - each defList node has def as first child,
    // and next defList as def's sibling
    TreeNode* defList = defListNode;
    while (defList && defList->type == NODE_DEFLIST) {
        TreeNode* def = defList->firstChild;
        if (def && def->type == NODE_DEF) {
            // Def -> Specifier DecList SEMI
            TreeNode* specifier = def->firstChild;
            Type baseType = analyzeSpecifier(specifier);

            if (baseType) {
                TreeNode* decList = specifier->nextSibling;
                if (decList && decList->type == NODE_DECLIST) {
                    // decList has either:
                    // - one dec (decList: dec)
                    // - dec, comma, decList (decList: dec COMMA decList)
                    TreeNode* decListIter = decList;
                    while (decListIter && decListIter->type == NODE_DECLIST) {
                        TreeNode* dec = decListIter->firstChild;
                        if (dec && dec->type == NODE_DEC) {
                            TreeNode* varDec = dec->firstChild;
                            if (varDec && varDec->type == NODE_VARDEC) {
                                // Get the variable name from VarDec
                                const char* fieldName = getIdentifierName(varDec);
                                if (fieldName) {
                                    // Build the complete type (including arrays)
                                    Type fieldType = analyzeVarDec(varDec, baseType, false);

                                    if (fieldType) {
                                        // Check for field redefinition (Error 15)
                                        if (fieldExists(head, fieldName)) {
                                            semanticError(15, def->lineNumber, "Redefined field");
                                            freeType(fieldType);
                                        } else {
                                            FieldList newField = createField(fieldName, fieldType);
                                            if (!head) {
                                                head = newField;
                                                tail = newField;
                                            } else {
                                                tail->tail = newField;
                                                tail = newField;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        // Move to next decList: skip comma if present
                        if (dec && dec->nextSibling && dec->nextSibling->type == NODE_COMMA) {
                            decListIter = dec->nextSibling->nextSibling;
                        } else {
                            decListIter = NULL;
                        }
                    }
                }
            }
        }
        // Next defList is the sibling of def
        defList = def ? def->nextSibling : NULL;
    }

    return head;
}

// ==================== Semantic Analysis Functions ====================

// Analyze a Specifier node - returns the Type
static Type analyzeSpecifier(TreeNode* node) {
    if (!node || node->type != NODE_SPECIFIER) return NULL;

    TreeNode* child = node->firstChild;
    if (!child) return NULL;

    if (child->type == NODE_TYPE) {
        // Basic type: int or float
        if (strcmp(child->name, "int") == 0) {
            return createBasicType(BASIC_INT);
        } else if (strcmp(child->name, "float") == 0) {
            return createBasicType(BASIC_FLOAT);
        }
    } else if (child->type == NODE_STRUCTSPECIFIER) {
        // Struct specifier: two cases
        // Case 1: struct Tag { ... } (definition)
        // Case 2: struct Tag (reference)

        TreeNode* structChild = child->firstChild; // NODE_STRUCT
        if (!structChild) return NULL;

        TreeNode* tagOrOptTag = structChild->nextSibling;

        if (tagOrOptTag) {
            // Check if this is a definition (has LC) or just a reference
            TreeNode* lcNode = tagOrOptTag->nextSibling;

            if (lcNode && lcNode->type == NODE_LC) {
                // Case 1: Struct definition - struct Tag { DefList }
                const char* structName = NULL;
                if (tagOrOptTag->type == NODE_OPTTAG) {
                    structName = getIdentifierName(tagOrOptTag);
                }

                if (structName) {
                    // Check if struct name is already defined (Error 16)
                    Symbol* existing = lookupSymbol(structName);
                    if (existing) {
                        semanticError(16, child->lineNumber, "Redefined structure");
                        // Continue processing but don't reinsert
                    } else {
                        // Insert into symbol table first (so struct can reference itself if needed)
                        Symbol* structSym = insertSymbol(structName, SYMBOL_STRUCT);
                        // We'll fill in the type after parsing fields
                        structSym->info.structType = NULL;
                    }

                    // Parse the defList to get fields
                    TreeNode* defList = lcNode->nextSibling;
                    FieldList fields = NULL;
                    if (defList && defList->type == NODE_DEFLIST) {
                        fields = analyzeDefListForFields(defList, child->lineNumber);
                    }

                    // Create the struct type
                    Type structType = createStructType(fields);

                    // Update the symbol table entry
                    if (existing == NULL) {
                        Symbol* structSym = lookupSymbol(structName);
                        if (structSym) {
                            structSym->info.structType = structType;
                        }
                    }

                    // Return a copy of the type (or just return it - manage memory carefully!)
                    // For simplicity, we'll return the type, but note that symbol table also has a reference
                    // You might want to implement reference counting or be careful with freeType
                    return structType;
                }
            } else {
                // Case 2: Struct reference - struct Tag
                const char* structName = NULL;
                if (tagOrOptTag->type == NODE_TAG) {
                    structName = getIdentifierName(tagOrOptTag);
                } else if (tagOrOptTag->type == NODE_OPTTAG) {
                    structName = getIdentifierName(tagOrOptTag);
                }

                if (structName) {
                    // Look up the struct in symbol table
                    Symbol* structSym = lookupSymbol(structName);
                    if (!structSym || structSym->kind != SYMBOL_STRUCT) {
                        // Error 17: Undefined struct
                        semanticError(17, child->lineNumber, "Undefined structure");
                        return NULL;
                    }

                    if (!structSym->info.structType) {
                        // Struct was declared but not fully defined?
                        semanticError(17, child->lineNumber, "Undefined structure");
                        return NULL;
                    }

                    // Return the struct type
                    // Note: you might want to make a copy here, but for simplicity we return the reference
                    return structSym->info.structType;
                }
            }
        }
    }

    return NULL;
}

// Process a VarDec node and insert into symbol table
static void processVarDec(TreeNode* varDecNode, Type baseType, int lineNo) {
    if (!varDecNode || varDecNode->type != NODE_VARDEC) return;

    // Get the variable name
    const char* varName = getIdentifierName(varDecNode);
    if (!varName) return;

    // Build the complete type
    Type varType = analyzeVarDec(varDecNode, baseType, false);
    if (!varType) return;

    // Check if already defined (Error 3)
    Symbol* existing = lookupSymbol(varName);
    if (existing && existing->depth == currentScopeDepth) {
        semanticError(3, lineNo, "Redefined variable");
        freeType(varType);
        return;
    }

    // Check if name conflicts with a struct (Error 3/16)
    if (existing && existing->kind == SYMBOL_STRUCT) {
        semanticError(3, lineNo, "Redefined variable");
        freeType(varType);
        return;
    }

    // Insert into symbol table
    Symbol* sym = insertSymbol(varName, SYMBOL_VARIABLE);

    // Make a copy of the type for the symbol table
    // For simplicity, we'll transfer ownership - be careful with freeType!
    sym->info.variableType = varType;
}

// Analyze a VarDec node - fills in type information
static Type analyzeVarDec(TreeNode* node, Type type, bool isParam) {
    if (!node || node->type != NODE_VARDEC) return NULL;

    // VarDec can be: T_ID or VarDec [ T_INT ]
    // We need to build the array type

    TreeNode* child = node->firstChild;
    if (!child) return NULL;

    if (child->type == NODE_ID) {
        // Simple variable, no array
        return type;
    } else if (child->type == NODE_VARDEC) {
        // Array - recursively analyze the inner VarDec
        Type elemType = analyzeVarDec(child, type, isParam);

        // Find the array size
        TreeNode* sibling = child->nextSibling;
        while (sibling) {
            if (sibling->type == NODE_INT) {
                int size = sibling->intVal;
                return createArrayType(elemType, size);
            }
            sibling = sibling->nextSibling;
        }
    }

    return type;
}

// Analyze an ExtDef node (external definition: global var, struct, function)
static void analyzeExtDef(TreeNode* node) {
    if (!node || node->type != NODE_EXTDEF) return;

    TreeNode* specifier = node->firstChild;
    if (!specifier) return;

    Type type = analyzeSpecifier(specifier);

    TreeNode* next = specifier->nextSibling;
    if (!next) {
        // Just a specifier (like struct definition without variables)
        if (type) freeType(type);
        return;
    }

    if (next->type == NODE_FUNDEC) {
        // Function definition
        TreeNode* funDec = next;
        TreeNode* idNode = funDec->firstChild; // T_ID
        const char* funcName = getIdentifierName(idNode);

        TreeNode* compSt = funDec->nextSibling;

        // ERROR type 2
        if (funcName && inFirstPass) {
            // First pass: insert function into symbol table
            Symbol* existing = lookupSymbol(funcName);
            if (existing) {
                semanticError(4, node->lineNumber, "Redefined function");
            } else {
                Symbol* sym = insertSymbol(funcName, SYMBOL_FUNCTION);
                // For now, just store the return type - params TODO later
                if (type) {
                    // Make a copy of the return type
                    sym->info.funcInfo.returnType = type;
                } else {
                    sym->info.funcInfo.returnType = NULL;
                }
                sym->info.funcInfo.params = NULL;
                sym->info.funcInfo.paramCount = 0;
                sym->info.funcInfo.defined = true;
            }
            // // Also process function body definitions in first pass!
            // if (compSt && compSt->type == NODE_COMPST) {
            //     analyzeCompSt(compSt);
            // }
        } else if (funcName && !inFirstPass) {
            // Second pass: analyze function body statements
            if (compSt && compSt->type == NODE_COMPST) {
                analyzeCompSt(compSt);
            }
        }
        // Don't free type - it's stored in symbol table
        return;

    } else if (next->type == NODE_VARDEC) {
        // Single global variable
        if (type && inFirstPass) {
            processVarDec(next, type, node->lineNumber);
        }

    } else if (next->type == NODE_EXTDECLIST) {
        // Multiple global variables
        if (type && inFirstPass) {
            TreeNode* varDec = next;
            while (varDec && varDec->type == NODE_VARDEC) {
                processVarDec(varDec, type, node->lineNumber);
                varDec = varDec->nextSibling;
            }
        }
    }

    if (type) freeType(type);
}

// Analyze a Def node (local definition)
static void analyzeDef(TreeNode* node) {
    if (!node || node->type != NODE_DEF) return;

    // Only process declarations in first pass
    if (!inFirstPass) return;

    TreeNode* specifier = node->firstChild;
    if (!specifier) return;

    Type type = analyzeSpecifier(specifier);
    if (!type) return;

    TreeNode* decList = specifier->nextSibling;
    if (!decList || decList->type != NODE_DECLIST) {
        freeType(type);
        return;
    }

    // Process each declaration in decList
    TreeNode* dec = decList->firstChild;
    while (dec) {
        if (dec->type == NODE_DEC) {
            TreeNode* varDec = dec->firstChild;
            if (varDec && varDec->type == NODE_VARDEC) {
                processVarDec(varDec, type, node->lineNumber);
            }
        }
        dec = dec->nextSibling;
    }

    freeType(type);
}

// Analyze a Stmt node
static void analyzeStmt(TreeNode* node) {
    if (!node || node->type != NODE_STMT) return;

    // Skip statements in first pass
    if (inFirstPass) return;

    TreeNode* child = node->firstChild;
    if (!child) return;

    Log(__func__, "child type: %s", nodeTypeName(child->type));

    switch (child->type) {
        case NODE_RETURN: {
            // Return statement
            TreeNode* expr = child->nextSibling;
            if (expr && expr->type == NODE_EXPR) {
                analyzeExpr(expr);
            }
            // TODO: Error 8 - check return type matches function
            break;
        }
        case NODE_IF: {
            // If statement
            TreeNode* lp = child->nextSibling; // (
            if (lp) {
                TreeNode* cond = lp->nextSibling; // expr
                if (cond && cond->type == NODE_EXPR) {
                    analyzeExpr(cond);
                    // TODO: Check condition is int
                }
            }
            break;
        }
        case NODE_WHILE: {
            // While statement
            TreeNode* lp = child->nextSibling; // (
            if (lp) {
                TreeNode* cond = lp->nextSibling; // expr
                if (cond && cond->type == NODE_EXPR) {
                    analyzeExpr(cond);
                    // TODO: Check condition is int
                }
            }
            break;
        }
        case NODE_COMPST:
            // Compound statement
            analyzeCompSt(child);
            break;
        default:
            // Expression statement
            if (child->type == NODE_EXPR) {
                analyzeExpr(child);
            }
            break;
    }
}

// Helper to traverse defList
static void traverseDefList(TreeNode* defList) {
    while (defList && defList->type == NODE_DEFLIST) {
        TreeNode* def = defList->firstChild;
        if (def && def->type == NODE_DEF) {
            analyzeDef(def);
        }
        // Next defList is the sibling of def
        defList = def ? def->nextSibling : NULL;
    }
}

// Helper to traverse stmtList
static void traverseStmtList(TreeNode* stmtList) {
    while (stmtList && stmtList->type == NODE_STMTLIST) {
        TreeNode* stmt = stmtList->firstChild;
        if (stmt && stmt->type == NODE_STMT) {
            analyzeStmt(stmt);
        }
        // Next stmtList is the sibling of stmt (due to how stmtList is built in syntax.y)
        stmtList = stmt ? stmt->nextSibling : NULL;
    }
}

// Analyze a CompSt node (compound statement)
static void analyzeCompSt(TreeNode* node) {
    if (!node || node->type != NODE_COMPST) return;

    // For Requirement 3.2 (nested scopes):
    // if (!inFirstPass) enterScope();

    TreeNode* child = node->firstChild; // should be LC
    if (child) child = child->nextSibling; // DefList (optional)

    // Analyze definitions (only in first pass)
    if (inFirstPass && child && child->type == NODE_DEFLIST) {
        traverseDefList(child);
        child = child->nextSibling; // move to StmtList
    } else if (!inFirstPass && child && child->type == NODE_DEFLIST) {
        // Skip DefList in second pass
        child = child->nextSibling; // move to StmtList
    }

    // Analyze statements (only in second pass)
    if (!inFirstPass && child && child->type == NODE_STMTLIST) {
        traverseStmtList(child);
    }

    // For Requirement 3.2 (nested scopes):
    // if (!inFirstPass) exitScope();
}

// Analyze an Expr node - returns the Type of the expression
static Type analyzeExpr(TreeNode* node) {
    Log(__func__, "entered, node: %p, node->type: %s, NODE_EXPR: %s",
        (void*)node,
        node ? nodeTypeName(node->type) : "NULL",
        nodeTypeName(NODE_EXPR));
    if (!node || node->type != NODE_EXPR) return NULL;
    // When meet Exp, then the node and children node will use varible or function
    // search the symbol table to identify these variable or function exists or not


    // Don't analyze expressions in first pass
    if (inFirstPass) return NULL;

    TreeNode* child = node->firstChild;
    if (!child) return NULL;

    TreeNode* sibling = child->nextSibling;

    // First, handle cases that start with non-ID nodes and don't require looking ahead much

    // Case A: Parentheses - ( expr )
    if (child->type == NODE_LP) {
        TreeNode* inner = child->nextSibling;
        if (inner && inner->type == NODE_EXPR) {
            return analyzeExpr(inner);
        }
        return NULL;
    }

    // Case B: Unary operations - !expr or -expr
    if (child->type == NODE_NOT || child->type == NODE_MINUS) {
        TreeNode* operand = child->nextSibling;
        if (operand) analyzeExpr(operand);
        return NULL;
    }

    // Case C: Integer literal
    if (child->type == NODE_INT) {
        return createBasicType(BASIC_INT);
    }

    // Case D: Float literal
    if (child->type == NODE_FLOAT) {
        return createBasicType(BASIC_FLOAT);
    }

    // Now handle all cases that start with an ID
    if (child->type == NODE_ID) {
        // We need to determine what kind of ID expression this is
        // Look at the siblings to decide

        // Subcase 1: Function call - ID ( args )
        // Check if any sibling is LP
        TreeNode* curr = sibling;
        int foundLP = 0;
        while (curr) {
            if (curr->type == NODE_LP) {
                foundLP = 1;
                break;
            }
            curr = curr->nextSibling;
        }

        if (foundLP) {
            const char* funcName = child->name;
            Symbol* sym = lookupSymbol(funcName);
            if (!sym || sym->kind != SYMBOL_FUNCTION) {
                // Error 2: Undefined function
                char msg[64];
                snprintf(msg, sizeof(msg), "Undefined function \"%s\"", funcName);
                semanticError(2, node->lineNumber, msg);
            }
            // TODO: Check arguments (Error 9)
            return NULL;
        }

        // Subcase 2: Assignment - ID = expr
        if (sibling && sibling->type == NODE_ASSIGNOP) {
            // First check if variable is defined
            const char* varName = child->name;
            Symbol* sym = lookupSymbol(varName);
            if (!sym) {
                // Error 1: Undefined variable
                char msg[64];
                snprintf(msg, sizeof(msg), "Undefined variable \"%s\"", varName);
                semanticError(1, node->lineNumber, msg);
            }
            // Analyze right side
            TreeNode* right = sibling->nextSibling;
            if (right) analyzeExpr(right);
            // TODO: Check type match (Error 5), check lvalue (Error 6)
            return NULL;
        }

        // Subcase 3: Array indexing - ID [ expr ]
        if (sibling && sibling->type == NODE_LB) {
            // First check if variable is defined
            const char* varName = child->name;
            Symbol* sym = lookupSymbol(varName);
            if (!sym) {
                // Error 1: Undefined variable
                char msg[64];
                snprintf(msg, sizeof(msg), "Undefined variable \"%s\"", varName);
                semanticError(1, node->lineNumber, msg);
            }
            // Analyze index
            TreeNode* index = sibling->nextSibling;
            if (index && index->type == NODE_EXPR) {
                analyzeExpr(index);
            }
            // TODO: Error 10 (non-array), Error 12 (non-int index)
            return NULL;
        }

        // Subcase 4: Struct member access - ID . ID
        if (sibling && sibling->type == NODE_DOT) {
            // First check if variable is defined
            const char* varName = child->name;
            Symbol* sym = lookupSymbol(varName);
            if (!sym) {
                // Error 1: Undefined variable
                char msg[64];
                snprintf(msg, sizeof(msg), "Undefined variable \"%s\"", varName);
                semanticError(1, node->lineNumber, msg);
            }
            // TODO: Error 13 (non-struct), Error 14 (undefined field)
            return NULL;
        }

        // Subcase 5: Just a variable reference
        const char* varName = child->name;
        Symbol* sym = lookupSymbol(varName);
        if (!sym) {
            // Error 1: Undefined variable
            char msg[64];
            snprintf(msg, sizeof(msg), "Undefined variable \"%s\"", varName);
            semanticError(1, node->lineNumber, msg);
            return NULL;
        }
        if (sym->kind == SYMBOL_VARIABLE) {
            return sym->info.variableType;
        } else if (sym->kind == SYMBOL_FUNCTION) {
            // Function referenced without calling - could be an error
            return NULL;
        }
        return NULL;
    }

    // Now handle cases where the child is another expression (binary ops, etc.)

    // Case E: Assignment - expr = expr (non-ID left side)
    if (sibling && sibling->type == NODE_ASSIGNOP) {
        analyzeExpr(child);
        TreeNode* right = sibling->nextSibling;
        if (right) analyzeExpr(right);
        // TODO: Check type match (Error 5), check lvalue (Error 6)
        return NULL;
    }

    // Case F: Binary operations - expr OP expr
    if (sibling && (sibling->type == NODE_PLUS || sibling->type == NODE_MINUS ||
                     sibling->type == NODE_STAR || sibling->type == NODE_DIV ||
                     sibling->type == NODE_RELOP || sibling->type == NODE_AND ||
                     sibling->type == NODE_OR)) {
        analyzeExpr(child);
        TreeNode* right = sibling->nextSibling;
        if (right) analyzeExpr(right);
        // TODO: Check operand types (Error 7)
        return NULL;
    }

    // Case G: Array indexing - expr [ expr ] (non-ID left side)
    if (sibling && sibling->type == NODE_LB) {
        analyzeExpr(child);
        TreeNode* index = sibling->nextSibling;
        if (index && index->type == NODE_EXPR) {
            analyzeExpr(index);
        }
        // TODO: Error 10 (non-array), Error 12 (non-int index)
        return NULL;
    }

    // Case H: Struct member access - expr . ID (non-ID left side)
    if (sibling && sibling->type == NODE_DOT) {
        analyzeExpr(child);
        // TODO: Error 13 (non-struct), Error 14 (undefined field)
        return NULL;
    }

    // Recursively analyze all children for other cases
    TreeNode* curr = child;
    while (curr) {
        if (curr->type == NODE_EXPR) {
            analyzeExpr(curr);
        }
        curr = curr->nextSibling;
    }

    return NULL;
}

// ==================== Main Semantic Analysis Function ====================

// Helper to traverse extDefList with current pass setting
static void traverseExtDefList(TreeNode* extDefList) {
    while (extDefList && extDefList->type == NODE_EXTDEF_LIST) {
        TreeNode* extDef = extDefList->firstChild;
        if (extDef && extDef->type == NODE_EXTDEF) {
            analyzeExtDef(extDef);
        }
        // Next extDefList is the sibling of extDef
        extDefList = extDef ? extDef->nextSibling : NULL;
    }
}

void analyzeSemantics(TreeNode* root) {
    if (!root || root->type != NODE_PROGRAM) {
        fprintf(stderr, "Invalid root node for semantic analysis\n");
        return;
    }

    initSemantics();

    // First pass: collect all declarations
    inFirstPass = true;
    traverseExtDefList(root->firstChild);

    // Second pass: analyze function bodies and expressions
    inFirstPass = false;
    traverseExtDefList(root->firstChild);
}

// ==================== Initialization and Cleanup ====================

void initSemantics() {
    semanticErrorCount = 0;
    currentScopeDepth = 0;
    // Initialize hash table
    memset(hashTable, 0, sizeof(hashTable));
}

void cleanupSemantics() {
    // Free all symbols
    for (int i = 0; i < HASH_TABLE_SIZE; ++i) {
        Symbol* symbol = hashTable[i];
        while (symbol) {
            Symbol* next = symbol->next;
            // Free type information if needed
            if (symbol->kind == SYMBOL_VARIABLE) {
                freeType(symbol->info.variableType);
            } else if (symbol->kind == SYMBOL_FUNCTION) {
                freeType(symbol->info.funcInfo.returnType);
                // Free parameters
                FieldList param = symbol->info.funcInfo.params;
                while (param) {
                    FieldList nextParam = param->tail;
                    freeType(param->type);
                    free(param);
                    param = nextParam;
                }
            } else if (symbol->kind == SYMBOL_STRUCT) {
                freeType(symbol->info.structType);
            }
            free(symbol);
            symbol = next;
        }
        hashTable[i] = NULL;
    }
}
