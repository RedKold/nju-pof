#include "semantics.h"
#include "tree/tree.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>

#define DEBUG

#define STRUCTRUAL_EQUAL


// ==================== Symbol Table Configuration ====================

#define HASH_TABLE_SIZE 16384 // 2^14, as recommended in the manual

// ==================== Global Variables ====================

static Symbol *hashTable[HASH_TABLE_SIZE] = {NULL};
static int semanticErrorCount = 0;
static int currentScopeDepth = 0; // for Requirement 3.2 (nested scopes)
static bool inFirstPass =
    false; // true = collecting declarations, false = checking usage
static Symbol *currentFunction =
    NULL; // track current function for return type checking

// ==================== Forward Declarations ====================

static Type analyzeSpecifier(TreeNode *node);
static Type analyzeVarDec(TreeNode *node, Type type, bool isParam);
static FieldList analyzeDefListForFields(TreeNode *defListNode, int structLine);
static void analyzeExtDef(TreeNode *node);
static void analyzeDef(TreeNode *node);
static void analyzeStmt(TreeNode *node);
static Type analyzeExpr(TreeNode *node);
static void analyzeCompSt(TreeNode *node);
static void processVarDec(TreeNode *varDecNode, Type baseType, int lineNo);
static void traverseExtDefList(TreeNode *extDefList);
static bool typeEqual(Type t1, Type t2);
static bool fieldListsEqual(FieldList f1, FieldList f2);
static void freeType(Type type);
static void freeFieldList(FieldList field);
static void processFunctionParams(TreeNode *funDecNode, Symbol *funcSym);
static Type copyType(Type type);

// ==================== Hash Function (P.J. Weinberger) ====================

static unsigned int hashpjw(const char *name) {
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
static Symbol *insertSymbol(const char *name, SymbolKind kind) {
  unsigned int hash = hashpjw(name);

  Symbol *symbol = (Symbol *)malloc(sizeof(Symbol));
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

static void Log(const char *func, const char *msg, ...) {
#ifdef DEBUG
  va_list args;
  va_start(args, msg);
  printf("[%s] ", func);
  vprintf(msg, args);
  printf("\n");
  va_end(args);
#endif
}

// Lookup a symbol in the hash table
static Symbol *lookupSymbol(const char *name) {
  unsigned int hash = hashpjw(name);
  Log(__func__, "Finding name: %s", name);
  Symbol *symbol = hashTable[hash];

  // For Requirement 3.2: find the symbol with the highest depth (innermost
  // scope)
  Symbol *found = NULL;
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

  if (found) {
    Log(__func__, "Name %s founded", name);
  } else {
    Log(__func__, "Not found");
  }

  return found;
}

// Remove all symbols at the current scope depth (for Requirement 3.2)
static void exitScope() {
  for (int i = 0; i < HASH_TABLE_SIZE; ++i) {
    Symbol **symbolPtr = &hashTable[i];
    while (*symbolPtr) {
      if ((*symbolPtr)->depth == currentScopeDepth) {
        Symbol *temp = *symbolPtr;
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
static void enterScope() { currentScopeDepth++; }

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
static FieldList createField(const char *name, Type type) {
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

// Check if two field lists are equivalent (same names, same types, same order)
static bool fieldListsEqual(FieldList f1, FieldList f2) {
  Log(__func__, "-------------- CHECKING FILED EQUAL -----------");
  while (f1 && f2) {

    // Check if field names are the same
#ifndef STRUCTRUAL_EQUAL
    if (strcmp(f1->name, f2->name) != 0) {
      return false;
    }
#endif

    // Check if field types are the same
    if (!typeEqual(f1->type, f2->type)) {
      return false;
    }
    // Move to next fields
    f1 = f1->tail;
    f2 = f2->tail;
  }
  // Both should be NULL at the end (same number of fields)
  return (f1 == NULL && f2 == NULL);
}

// Check two types for equivalence (structural equivalence for structs)
static bool typeEqual(Type t1, Type t2) {
  if (!t1 || !t2)
    return false;
  if (t1->kind != t2->kind)
    return false;

  switch (t1->kind) {
  case TYPE_BASIC:
    return t1->u.basic == t2->u.basic;
  case TYPE_ARRAY:
    // For arrays, same element type and same size
    return (t1->u.array.size == t2->u.array.size) &&
           typeEqual(t1->u.array.elem, t2->u.array.elem);
  case TYPE_STRUCTURE:
    // Check structural equivalence: same fields in same order with same types
    return fieldListsEqual(t1->u.structure, t2->u.structure);
  default:
    return false;
  }
}

// Free a field list (helper function)
static void freeFieldList(FieldList field) {
  while (field) {
    FieldList next = field->tail;
    // freeType(field->type);
    free(field);
    field = next;
  }
}

// Free a type (helper function)
static void freeType(Type type) {
  if (!type)
    return;
  // Don't free struct types - they're managed by cleanupSemantics
  if (type->kind == TYPE_STRUCTURE) {
    return;
  }
  switch (type->kind) {
  case TYPE_ARRAY:
    freeType(type->u.array.elem);
    break;
  default:
    break;
  }
  free(type);
}

// Check LValue
/**
 * 检查一个表达式节点是否为合法的左值 (L-value)
 * 合法情况：
 * 1. Exp -> ID
 * 2. Exp -> Exp LB Exp RB  (数组访问)
 * 3. Exp -> Exp DOT ID     (结构体成员访问)
 */
static bool isLValue(TreeNode *expNode) {
  if (!expNode || expNode->type != NODE_EXPR)
    return false;

  TreeNode *child = expNode->firstChild;
  if (!child)
    return false;

  // 情况 1: Exp -> ID
  if (child->type == NODE_ID && child->nextSibling == NULL) {
    return true;
  }

  // 情况 2 & 3: 第一个子节点也是 Exp
  if (child->type == NODE_EXPR) {
    TreeNode *op = child->nextSibling;
    if (op) {
      // Exp -> Exp LB Exp RB
      if (op->type == NODE_LB)
        return true;
      // Exp -> Exp DOT ID
      if (op->type == NODE_DOT)
        return true;
    }
  }

  // 其他情况（如 NODE_INT, NODE_FLOAT, 或二元运算 Exp + Exp）均非法
  return false;
}

// ==================== Error Reporting ====================

void semanticError(int errorType, int lineNumber, const char *description) {
  printf("Error type %d at Line %d: %s.\n", errorType, lineNumber, description);
  semanticErrorCount++;
}

int getSemanticErrorCount() { return semanticErrorCount; }

// ==================== AST Traversal Helper Functions ====================

// Get the identifier name from a node (usually a T_ID child)
static const char *getIdentifierName(TreeNode *node) {
  if (!node)
    return NULL;
  // If this is an ID node
  if (node->type == NODE_ID) {
    return node->name;
  }
  // Otherwise look for an ID child
  TreeNode *child = node->firstChild;
  while (child) {
    const char *name = getIdentifierName(child);
    if (name)
      return name;
    child = child->nextSibling;
  }
  return NULL;
}

// Check if a field name already exists in a field list
static bool fieldExists(FieldList fields, const char *name) {
  while (fields) {
    if (strcmp(fields->name, name) == 0) {
      return true;
    }
    fields = fields->tail;
  }
  return false;
}

// Analyze a DefList to extract struct fields
static FieldList analyzeDefListForFields(TreeNode *defListNode,
                                         int structLine) {
  FieldList head = NULL;
  FieldList tail = NULL;

  // Traverse defList properly - each defList node has def as first child,
  // and next defList as def's sibling
  TreeNode *defList = defListNode;
  while (defList && defList->type == NODE_DEFLIST) {
    TreeNode *def = defList->firstChild;
    if (def && def->type == NODE_DEF) {
      // Def -> Specifier DecList SEMI
      TreeNode *specifier = def->firstChild;
      Type baseType = analyzeSpecifier(specifier);

      if (baseType) {
        TreeNode *decList = specifier->nextSibling;
        if (decList && decList->type == NODE_DECLIST) {
          // decList has either:
          // - one dec (decList: dec)
          // - dec, comma, decList (decList: dec COMMA decList)
          TreeNode *decListIter = decList;
          while (decListIter && decListIter->type == NODE_DECLIST) {
            TreeNode *dec = decListIter->firstChild;
            if (dec && dec->type == NODE_DEC) {
              TreeNode *varDec = dec->firstChild;
              if (varDec && varDec->type == NODE_VARDEC) {
                // Get the variable name from VarDec
                const char *fieldName = getIdentifierName(varDec);
                if (fieldName) {
                  // Build the complete type (including arrays)
                  Type fieldType = analyzeVarDec(varDec, baseType, false);

                  if (fieldType) {
                    // Check for field redefinition (Error 15)
                    if (inFirstPass && fieldExists(head, fieldName)) {
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
            if (dec && dec->nextSibling &&
                dec->nextSibling->type == NODE_COMMA) {
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
static Type analyzeSpecifier(TreeNode *node) {
  if (!node || node->type != NODE_SPECIFIER)
    return NULL;

  TreeNode *child = node->firstChild;
  if (!child)
    return NULL;

  if (child->type == NODE_TYPE) {
    // Basic type: int or float
    if (strcmp(child->name, "int") == 0) {
      Type t = createBasicType(BASIC_INT);
      Log(__func__, "Returning basic int type %p, kind=%d", (void*)t, t->kind);
      return t;
    } else if (strcmp(child->name, "float") == 0) {
      Type t = createBasicType(BASIC_FLOAT);
      Log(__func__, "Returning basic float type %p, kind=%d", (void*)t, t->kind);
      return t;
    }
  } else if (child->type == NODE_STRUCTSPECIFIER) {
    // Struct specifier: two cases
    // Case 1: struct Tag { ... } (definition)
    // Case 2: struct Tag (reference)

    TreeNode *structChild = child->firstChild; // NODE_STRUCT
    if (!structChild)
      return NULL;

    TreeNode *tagOrOptTag = structChild->nextSibling;

    Log(__func__, "--------------CHECKING TAGS------------");

    if (tagOrOptTag) {
      // Check if this is a definition (has LC) or just a reference
      TreeNode *lcNode = tagOrOptTag->nextSibling;

      if (lcNode && lcNode->type == NODE_LC) {
        Log(__func__, "--------------CASE 1-----------");
        // Case 1: Struct definition - struct Tag { DefList } in that way this
        // is a OptTag
        const char *structName = NULL;
        if (tagOrOptTag->type == NODE_OPTTAG) {
          structName = getIdentifierName(tagOrOptTag);
        }

        // Parse the defList to get fields (do this even in second pass to
        // return the type)
        TreeNode *defList = lcNode->nextSibling;
        FieldList fields = NULL;
        if (defList && defList->type == NODE_DEFLIST) {
          fields = analyzeDefListForFields(defList, child->lineNumber);
        }

        // Create the struct type
        Type structType = createStructType(fields);

        if (structName &&
            inFirstPass) { // Only modify symbol table in first pass!
          // Check if struct name is already defined (Error 16)
          Symbol *existing = lookupSymbol(structName);
          if (existing) {
            semanticError(16, child->lineNumber, "Redefined structure");
          } else {
            // Insert into symbol table first (so struct can reference itself if
            // needed)
            Symbol *structSym = insertSymbol(structName, SYMBOL_STRUCT);
            structSym->info.structType = copyType(structType); // Store a copy
          }
        }

        // Return the struct type
        Log(__func__, "Returning struct definition type %p, kind=%d", (void*)structType, structType->kind);
        return structType;
      } else {
        // Case 2: Struct reference - struct Tag
        Log(__func__, "--------------CASE 2-----------");
        const char *structName = NULL;
        if (tagOrOptTag->type == NODE_TAG) {
          structName = getIdentifierName(tagOrOptTag);
        } else if (tagOrOptTag->type == NODE_OPTTAG) {
          structName = getIdentifierName(tagOrOptTag);
        }

        if (structName) {
          Log(__func__, "struct Name is %s", structName);
          // Look up the struct in symbol table
          Symbol *structSym = lookupSymbol(structName);
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

          Log(__func__, "Returning struct reference type %p, kind=%d", (void*)structSym->info.structType, structSym->info.structType->kind);
          // Return the struct type
          // Note: you might want to make a copy here, but for simplicity we
          // return the reference
          return structSym->info.structType;
        }
      }
    }
  }

  return NULL;
}

// Process a VarDec node and insert into symbol table
static void processVarDec(TreeNode *varDecNode, Type baseType, int lineNo) {
  Log(__func__, "dealing with varDec, baseType=%p", (void*)baseType);
  if (baseType) {
    Log(__func__, "baseType kind=%d", baseType->kind);
  }
  if (!varDecNode || varDecNode->type != NODE_VARDEC)
    return;

  // Get the variable name
  const char *varName = getIdentifierName(varDecNode);
  if (!varName)
    return;

  // Build the complete type
  Type varType = analyzeVarDec(varDecNode, baseType, false);
  Log(__func__, "varName=%s, varType=%p, varType kind=%d", varName, (void*)varType, varType ? varType->kind : -1);
  if (!varType)
    return;

  // Check if already defined (Error 3)
  Symbol *existing = lookupSymbol(varName);
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
  Symbol *sym = insertSymbol(varName, SYMBOL_VARIABLE);

  // Make a copy of the type for the symbol table
  // For simplicity, we'll transfer ownership - be careful with freeType!
  sym->info.variableType = varType;
}

// Process function parameters and add them to the symbol table
static void processFunctionParams(TreeNode *funDecNode, Symbol *funcSym) {
  if (!funDecNode || funDecNode->type != NODE_FUNDEC)
    return;

  // FunDec structure: T_ID LP [VarList] RP
  TreeNode *child = funDecNode->firstChild; // T_ID
  if (!child)
    return;

  child = child->nextSibling; // LP
  if (!child)
    return;

  child = child->nextSibling; // VarList (first paramDec)

  FieldList *paramTailPtr = &funcSym->info.funcInfo.params;
  int paramCount = 0;

  // Traverse VarList - it's a list of paramDec nodes connected by siblings
  while (child && child->type == NODE_PARAMDEC) {
    TreeNode *paramDec = child;

    // ParamDec has Specifier and VarDec as children
    TreeNode *specifier = paramDec->firstChild;
    if (!specifier) {
      child = child->nextSibling;
      continue;
    }

    Type paramType = analyzeSpecifier(specifier);
    if (!paramType) {
      child = child->nextSibling;
      continue;
    }

    TreeNode *varDec = specifier->nextSibling;
    if (!varDec || varDec->type != NODE_VARDEC) {
      freeType(paramType);
      child = child->nextSibling;
      continue;
    }

    // Get parameter name
    const char *paramName = getIdentifierName(varDec);
    if (!paramName) {
      freeType(paramType);
      child = child->nextSibling;
      continue;
    }

    // Build the complete parameter type (including arrays)
    Type completeParamType = analyzeVarDec(varDec, paramType, true);
    // Free the original paramType since analyzeVarDec makes a copy
    freeType(paramType);

    if (!completeParamType) {
      child = child->nextSibling;
      continue;
    }

    // Check if parameter is already defined (in the same scope)
    Symbol *existing = lookupSymbol(paramName);
    if (existing && existing->depth == currentScopeDepth) {
      semanticError(3, paramDec->lineNumber, "Redefined variable");
      freeType(completeParamType);
    } else {
      // Add parameter to symbol table
      Symbol *paramSym = insertSymbol(paramName, SYMBOL_VARIABLE);
      paramSym->info.variableType = completeParamType;

      // Add to function's parameter list
      FieldList newParam = createField(paramName, completeParamType);
      *paramTailPtr = newParam;
      paramTailPtr = &newParam->tail;
      paramCount++;
    }

    child = child->nextSibling; // next paramDec or RP
  }

  funcSym->info.funcInfo.paramCount = paramCount;
}

// Helper to copy a type
static Type copyType(Type type) {
  if (!type)
    return NULL;

  Type newType = (Type)malloc(sizeof(struct Type_));
  if (!newType) {
    fprintf(stderr, "Out of memory\n");
    exit(1);
  }

  newType->kind = type->kind;
  switch (type->kind) {
  case TYPE_BASIC:
    newType->u.basic = type->u.basic;
    break;
  case TYPE_ARRAY:
    newType->u.array.elem = copyType(type->u.array.elem);
    newType->u.array.size = type->u.array.size;
    break;
  case TYPE_STRUCTURE:
    // For structs, we just copy the reference (field list is shared)
    newType->u.structure = type->u.structure;
    break;
  }

  return newType;
}

// Analyze a VarDec node - fills in type information
static Type analyzeVarDec(TreeNode *node, Type type, bool isParam) {
  if (!node || node->type != NODE_VARDEC)
    return NULL;

  // VarDec can be: T_ID or VarDec [ T_INT ]
  // We need to build the array type

  TreeNode *child = node->firstChild;
  if (!child)
    return NULL;

  if (child->type == NODE_ID) {
    // Simple variable, no array - make a copy of the type
    return copyType(type);
  } else if (child->type == NODE_VARDEC) {
    // Array - recursively analyze the inner VarDec
    Type elemType = analyzeVarDec(child, type, isParam);

    // Find the array size
    TreeNode *sibling = child->nextSibling;
    while (sibling) {
      if (sibling->type == NODE_INT) {
        int size = sibling->intVal;
        return createArrayType(elemType, size);
      }
      sibling = sibling->nextSibling;
    }
    // If no size found, return the element type (which is already a copy)
    return elemType;
  }

  // Default: return a copy
  return copyType(type);
}

// Analyze an ExtDef node (external definition: global var, struct, function)
static void analyzeExtDef(TreeNode *node) {
  if (!node || node->type != NODE_EXTDEF)
    return;

  TreeNode *specifier = node->firstChild;
  if (!specifier)
    return;

  Type type = analyzeSpecifier(specifier);

  TreeNode *next = specifier->nextSibling;
  if (!next) {
    // Just a specifier (like struct definition without variables)
    if (type && type->kind != TYPE_STRUCTURE)
      freeType(type);
    return;
  }

  if (next->type == NODE_FUNDEC) {
    // Function definition
    TreeNode *funDec = next;
    TreeNode *idNode = funDec->firstChild; // T_ID
    const char *funcName = getIdentifierName(idNode);

    TreeNode *compSt = funDec->nextSibling;

    // ERROR type 2
    if (funcName && inFirstPass) {
      // First pass: insert function into symbol table
      Symbol *existing = lookupSymbol(funcName);
      if (existing) {
        semanticError(4, node->lineNumber, "Redefined function");

        // Free the type since we're not using it
        if (type && type->kind != TYPE_STRUCTURE)
          freeType(type);
        type = NULL;

      } else {
        Symbol *sym = insertSymbol(funcName, SYMBOL_FUNCTION);
        // use copy
        if (type) {
          sym->info.funcInfo.returnType = copyType(type);
        } else {
          sym->info.funcInfo.returnType = NULL;
        }
        sym->info.funcInfo.params = NULL;
        sym->info.funcInfo.paramCount = 0;
        sym->info.funcInfo.defined = true;

        // Process function parameters and add them to symbol table
        // Do this BEFORE processing the function body!
        processFunctionParams(funDec, sym);
      }
      // Also process function body definitions in first pass!
      if (compSt && compSt->type == NODE_COMPST) {
        analyzeCompSt(compSt);
      }
    } else if (funcName && !inFirstPass) {
      // Second pass: analyze function body statements
      if (compSt && compSt->type == NODE_COMPST) {
        // Set current function for return type checking
        currentFunction = lookupSymbol(funcName);
        analyzeCompSt(compSt);
        currentFunction = NULL;
      }
      // Free the type since we didn't store it anywhere in the second pass
      if (type && type->kind != TYPE_STRUCTURE)
        freeType(type);
    }
    // Don't free type in first pass - it's stored in the symbol table
    return;

  } else if (next->type == NODE_VARDEC) {
    // Single global variable
    if (type && inFirstPass) {
      processVarDec(next, type, node->lineNumber);
    }

  } else if (next->type == NODE_EXTDECLIST) {
    // Multiple global variables
    if (type && inFirstPass) {
      TreeNode *varDec = next;
      while (varDec && varDec->type == NODE_VARDEC) {
        processVarDec(varDec, type, node->lineNumber);
        varDec = varDec->nextSibling;
      }
    }
  } else if (next->type == NODE_COMPST) {
    if (type && inFirstPass) {
      analyzeCompSt(next);
    }
  }

  if (type && type->kind != TYPE_STRUCTURE)
    freeType(type);
}

// Analyze a Def node (local definition)
static void analyzeDef(TreeNode *node) {
  if (!node || node->type != NODE_DEF)
    return;

  // Only process declarations in first pass
  if (!inFirstPass)
    return;

  TreeNode *specifier = node->firstChild;
  if (!specifier)
    return;

  Type type = analyzeSpecifier(specifier);
  if (!type)
    return;

  TreeNode *decList = specifier->nextSibling;
  if (!decList || decList->type != NODE_DECLIST) {
    if (type->kind != TYPE_STRUCTURE)
      freeType(type);
    return;
  }

  // Process each declaration in decList
  TreeNode *dec = decList->firstChild;
  while (dec) {
    if (dec->type == NODE_DEC) {
      TreeNode *varDec = dec->firstChild;
      if (varDec && varDec->type == NODE_VARDEC) {
        processVarDec(varDec, type, node->lineNumber);
      }
    }
    dec = dec->nextSibling;
  }

  freeType(type);
}

// Analyze a Stmt node
static void analyzeStmt(TreeNode *node) {
  if (!node || node->type != NODE_STMT)
    return;

  // Skip statements in first pass
  if (inFirstPass)
    return;

  TreeNode *child = node->firstChild;
  if (!child)
    return;

  Log(__func__, "child type: %s", nodeTypeName(child->type));

  switch (child->type) {
  case NODE_RETURN: {
    // Return statement
    TreeNode *expr = child->nextSibling;
    Type return_val_type = NULL;
    if (expr && expr->type == NODE_EXPR) {
      return_val_type = analyzeExpr(expr);
    }

    // Error 8 - check return type matches function
    if (currentFunction && currentFunction->kind == SYMBOL_FUNCTION) {
      Type expected_type = currentFunction->info.funcInfo.returnType;

      // Both types exist but don't match

      // char msg[64] = "Type mismatched for return";

      if (expected_type && return_val_type &&
          !typeEqual(expected_type, return_val_type)) {
        semanticError(8, node->lineNumber, "Type mismatched for return");
      }
      // Function returns something but we return nothing
      else if (expected_type && !return_val_type) {
        semanticError(8, node->lineNumber, "Type mismatched for return");
      }
      // Function returns nothing but we return something
      else if (!expected_type && return_val_type) {
        semanticError(8, node->lineNumber, "Type mismatched for return");
      }
    }

    break;
  }
  case NODE_IF: {
    // If statement
    TreeNode *lp = child->nextSibling; // (
    if (lp) {
      TreeNode *cond = lp->nextSibling; // expr
      if (cond && cond->type == NODE_EXPR) {
        analyzeExpr(cond);
        // TODO: Check condition is int
      }
    }
    break;
  }
  case NODE_WHILE: {
    // While statement
    TreeNode *lp = child->nextSibling; // (
    if (lp) {
      TreeNode *cond = lp->nextSibling; // expr
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
static void traverseDefList(TreeNode *defList) {
  while (defList && defList->type == NODE_DEFLIST) {
    TreeNode *def = defList->firstChild;
    if (def && def->type == NODE_DEF) {
      analyzeDef(def);
    }
    // Next defList is the sibling of def
    defList = def ? def->nextSibling : NULL;
  }
}

// Helper to traverse stmtList
static void traverseStmtList(TreeNode *stmtList) {
  while (stmtList && stmtList->type == NODE_STMTLIST) {
    TreeNode *stmt = stmtList->firstChild;
    if (stmt && stmt->type == NODE_STMT) {
      analyzeStmt(stmt);
    }
    // Next stmtList is the sibling of stmt (due to how stmtList is built in
    // syntax.y)
    stmtList = stmt ? stmt->nextSibling : NULL;
  }
}

// Analyze a CompSt node (compound statement)
static void analyzeCompSt(TreeNode *node) {
  if (!node || node->type != NODE_COMPST)
    return;

  // For Requirement 3.2 (nested scopes):
  // if (!inFirstPass) enterScope();

  TreeNode *child = node->firstChild; // should be LC
  if (child)
    child = child->nextSibling; // DefList (optional)

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
static Type analyzeExpr(TreeNode *node) {
  Log(__func__, "entered, node: %p, node->type: %s, NODE_EXPR: %s",
      (void *)node, node ? nodeTypeName(node->type) : "NULL",
      nodeTypeName(NODE_EXPR));
  if (!node || node->type != NODE_EXPR)
    return NULL;
  // When meet Exp, then the node and children node will use varible or function
  // search the symbol table to identify these variable or function exists or
  // not

  // Don't analyze expressions in first pass
  if (inFirstPass)
    return NULL;

  TreeNode *child = node->firstChild;
  if (!child)
    return NULL;

  TreeNode *sibling = child->nextSibling;

  // printf("Children's type: %s\n", nodeTypeName(child->type));
  // if (sibling) {
  //   printf("Sibling's type: %s\n", nodeTypeName(sibling->type));
  // }

  // First, handle cases that start with non-ID nodes and don't require looking
  // ahead much

  // Case A: Parentheses - ( expr )
  if (child->type == NODE_LP) {
    Log(__func__, "CASE A:\n");
    TreeNode *inner = child->nextSibling;
    if (inner && inner->type == NODE_EXPR) {
      return analyzeExpr(inner);
    }
    return NULL;
  }

  // Case B: Unary operations - !expr or -expr
  if (child->type == NODE_NOT || child->type == NODE_MINUS) {
    Log(__func__, "CASE B:\n");
    TreeNode *operand = child->nextSibling;
    if (operand)
      analyzeExpr(operand);
    return NULL;
  }

  // Case C: Integer literal
  if (child->type == NODE_INT) {
    // Log(__func__, "CASE C:\n");
    return createBasicType(BASIC_INT);
  }

  // Case D: Float literal
  if (child->type == NODE_FLOAT) {
    Log(__func__, "CASE D:\n");
    return createBasicType(BASIC_FLOAT);
  }

  // Case Other: Start with a ID(EXPR has the id!)
  // Now handle all cases that start with an ID
  if (child->type == NODE_ID) {
    Log(__func__, "CASE Other:\n");
    // Log(__func__, "Sibling's type: %s", nodeTypeName(sibling->type));
    // We need to determine what kind of ID expression this is
    // Look at the siblings to decide

    // Subcase 1: Function call - ID ( args )
    // Check if any sibling is LP
    TreeNode *curr = sibling;
    int foundLP = 0;
    while (curr) {
      if (curr->type == NODE_LP) {
        foundLP = 1;
        break;
      }
      curr = curr->nextSibling;
    }

    if (foundLP) {
      const char *funcName = child->name;
      Symbol *sym = lookupSymbol(funcName);
      if (!sym) {
        // Error 2: Undefined function
        char msg[64];
        snprintf(msg, sizeof(msg), "Undefined function \"%s\"", funcName);
        semanticError(2, node->lineNumber, msg);
        return NULL;
      }
      if (sym->kind != SYMBOL_FUNCTION) {
        char msg[64];
        snprintf(msg, sizeof(msg), "\"%s\" is not a function", sym->name);
        semanticError(11, node->lineNumber, msg);
        return NULL;
      }

      Log(__func__, "Checking arguments...");

      // Now check arguments (Error 9)
      TreeNode *argsNode = curr->nextSibling; // should be NODE_ARGS or RP
      int argCount = 0;
      FieldList paramIter = sym->info.funcInfo.params;

      // Traverse args list
      TreeNode *argList = argsNode;
      while (argList && argList->type == NODE_ARGS) {
        TreeNode *argExpr = argList->firstChild;
        if (argExpr && argExpr->type == NODE_EXPR) {
          Type argType = analyzeExpr(argExpr);

          // Check type if we have a parameter
          if (paramIter) {
            if (!typeEqual(argType, paramIter->type)) {
              semanticError(9, node->lineNumber, "Function argument mismatch");
            }
            paramIter = paramIter->tail;
          }

          argCount++;
        }

        // Move to next arg: if there's a COMMA, skip it and get next ARGS
        if (argExpr && argExpr->nextSibling &&
            argExpr->nextSibling->type == NODE_COMMA) {
          argList = argExpr->nextSibling->nextSibling;
        } else {
          argList = NULL;
        }
      }

      // Check parameter count matches
      if (argCount != sym->info.funcInfo.paramCount) {
        semanticError(9, node->lineNumber, "Function argument mismatch");
      }

      return sym->info.funcInfo.returnType;
    }

    // Subcase 3: Array indexing - ID [ expr ]
    if (sibling && sibling->type == NODE_LB) {
      // First check if variable is defined
      const char *varName = child->name;
      Symbol *sym = lookupSymbol(varName);
      Type arrayType = NULL;
      if (!sym) {
        // Error 1: Undefined variable
        char msg[64];
        snprintf(msg, sizeof(msg), "Undefined variable \"%s\"", varName);
        semanticError(1, node->lineNumber, msg);
      } else if (sym->kind == SYMBOL_VARIABLE) {
        arrayType = sym->info.variableType;
        // Error 10: Check if it's an array
        if (!arrayType || arrayType->kind != TYPE_ARRAY) {
          char msg[64];
          snprintf(msg, sizeof(msg), "\"%s\" is not an array", sym->name);
          semanticError(10, node->lineNumber, msg);
        }
      }
      // Analyze index
      TreeNode *index = sibling->nextSibling;
      Type indexType = NULL;
      if (index && index->type == NODE_EXPR) {
        indexType = analyzeExpr(index);
      }
      // Error 12: Check if index is integer
      if (indexType &&
          (indexType->kind != TYPE_BASIC || indexType->u.basic != BASIC_INT)) {
        semanticError(12, node->lineNumber, "Non-integer used as array index");
      }

      // Return the element type if we have an array
      if (arrayType && arrayType->kind == TYPE_ARRAY) {
        return arrayType->u.array.elem;
      }
      return NULL;
    }

    // Subcase 4: Struct member access - ID . ID
    if (sibling && sibling->type == NODE_DOT) {
      // First check if variable is defined
      const char *varName = child->name;
      Symbol *sym = lookupSymbol(varName);
      Type structType = NULL;
      if (!sym) {
        // Error 1: Undefined variable
        char msg[64];
        snprintf(msg, sizeof(msg), "Undefined variable \"%s\"", varName);
        semanticError(1, node->lineNumber, msg);
      } else if (sym->kind == SYMBOL_VARIABLE) {
        structType = sym->info.variableType;
        // Debug print
        if (structType) {
          Log(__func__, "Variable %s has type kind %d", varName, structType->kind);
        } else {
          Log(__func__, "Variable %s has NULL type", varName);
        }
        // Error 13: Check if it's a struct
        if (!structType || structType->kind != TYPE_STRUCTURE) {
          semanticError(13, node->lineNumber, "Dot operator on non-struct");
        }
      }

      // Get field name
      TreeNode *idNode = sibling->nextSibling;
      if (idNode && idNode->type == NODE_ID && structType &&
          structType->kind == TYPE_STRUCTURE) {
        const char *fieldName = idNode->name;
        // Error 14: Check if field exists
        FieldList fieldIter = structType->u.structure;
        bool found = false;
        while (fieldIter) {
          if (strcmp(fieldIter->name, fieldName) == 0) {
            found = true;
            return fieldIter->type; // return field type
          }
          fieldIter = fieldIter->tail;
        }
        if (!found) {
          char msg[64];
          snprintf(msg, sizeof(msg), "Accessing undefined struct field \"%s\"",
                   fieldName);
          semanticError(14, node->lineNumber, msg);
        }
      }
      return NULL;
    }

    // Subcase 5: Just a variable reference
    const char *varName = child->name;
    Symbol *sym = lookupSymbol(varName);
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
    Log(__func__, "CASE E:\n");
    TreeNode *right = sibling->nextSibling;
    if (right)
      analyzeExpr(right);
    // TODO: Check type match (Error 5), check lvalue (Error 6)

    TreeNode *left = child;

    // Symbol* left_symbol = lookupSymbol(left->firstChild->name);
    Type left_type = analyzeExpr(left);
    Type right_type = analyzeExpr(right);

    assert(right);
    // Symbol* right_symbol = lookupSymbol(right->firstChild->name);
    // assert(right_symbol);

    if (!typeEqual(left_type, right_type)) {
      Log(__func__, "AssignOp between different type");
      char buf[256];
      if (left_type && right_type) {
        sprintf(buf, "Type mismatch in assignment");
        semanticError(5, node->lineNumber, buf);
      }
    }

    // check if a right-value be assigned
    // Need to analyse the left.

    if (!isLValue(left)) {
      char msg[64];
      snprintf(msg, sizeof(msg),
               "The left-hand side of an assignment must be a variable");
      semanticError(6, left->lineNumber, msg);
    }

    return left_type;
  }

  // Case F: Binary operations - expr OP expr
  if (sibling && (sibling->type == NODE_PLUS || sibling->type == NODE_MINUS ||
                  sibling->type == NODE_STAR || sibling->type == NODE_DIV ||
                  sibling->type == NODE_RELOP || sibling->type == NODE_AND ||
                  sibling->type == NODE_OR)) {
    Type t1 = analyzeExpr(child);
    Type t2 = analyzeExpr(sibling->nextSibling);

    if (t1 && t2) {
      if (t1->kind == TYPE_BASIC && t2->kind == TYPE_BASIC &&
          t1->u.basic == t2->u.basic) {
        return t1;
      } else {
        semanticError(7, node->lineNumber, "Type mismatched for oprands");
      }
    }
    // TODO: Check operand types (Error 7)
    return NULL;
  }

  // Case G: Array indexing - expr [ expr ] (non-ID left side)
  if (sibling && sibling->type == NODE_LB) {
    Type arrayType = analyzeExpr(child);
    // Error 10: Check if it's an array
    if (!arrayType || arrayType->kind != TYPE_ARRAY) {
      semanticError(10, node->lineNumber, "Array indexing on non-array");
    }
    TreeNode *index = sibling->nextSibling;
    Type indexType = NULL;
    if (index && index->type == NODE_EXPR) {
      indexType = analyzeExpr(index);
    }
    // Error 12: Check if index is integer
    if (indexType &&
        (indexType->kind != TYPE_BASIC || indexType->u.basic != BASIC_INT)) {
      semanticError(12, node->lineNumber, "Non-integer used as array index");
    }
    // Return element type if it's an array
    if (arrayType && arrayType->kind == TYPE_ARRAY) {
      return arrayType->u.array.elem;
    }
    return NULL;
  }

  // Case H: Struct member access - expr . ID (non-ID left side)
  if (sibling && sibling->type == NODE_DOT) {
    Type structType = analyzeExpr(child);
    // Error 13: Check if it's a struct
    if (!structType || structType->kind != TYPE_STRUCTURE) {
      semanticError(13, node->lineNumber, "Dot operator on non-struct");
    }

    // Get field name
    TreeNode *idNode = sibling->nextSibling;
    if (idNode && idNode->type == NODE_ID && structType &&
        structType->kind == TYPE_STRUCTURE) {
      const char *fieldName = idNode->name;
      // Error 14: Check if field exists
      FieldList fieldIter = structType->u.structure;
      bool found = false;
      while (fieldIter) {
        if (strcmp(fieldIter->name, fieldName) == 0) {
          found = true;
          return fieldIter->type; // return field type
        }
        fieldIter = fieldIter->tail;
      }
      if (!found) {
        char msg[64];
        snprintf(msg, sizeof(msg), "Accessing undefined struct field \"%s\"",
                 fieldName);
        semanticError(14, node->lineNumber, msg);
      }
    }
    return NULL;
  }

  // Recursively analyze all children for other cases
  TreeNode *curr = child;
  while (curr) {
    if (curr->type == NODE_EXPR) {
      analyzeExpr(curr);
    }
    curr = curr->nextSibling;
  }

  return NULL;
}

// ==================== Symbol Table Printing Functions ====================

static void printType(Type type) {
    if (!type) {
        printf("NULL");
        return;
    }
    switch (type->kind) {
        case TYPE_BASIC:
            printf("%s", type->u.basic == BASIC_INT ? "int" : "float");
            break;
        case TYPE_ARRAY:
            printf("array[");
            if (type->u.array.size > 0) {
                printf("%d", type->u.array.size);
            }
            printf("] of ");
            printType(type->u.array.elem);
            break;
        case TYPE_STRUCTURE:
            printf("struct { ");
            FieldList field = type->u.structure;
            while (field) {
                printf("%s: ", field->name);
                printType(field->type);
                if (field->tail) printf(", ");
                field = field->tail;
            }
            printf(" }");
            break;
        case TYPE_ERROR:
            printf("error");
            break;
    }
}

static void printFieldList(FieldList fields) {
    printf("(");
    FieldList field = fields;
    while (field) {
        printType(field->type);
        printf(" %s", field->name);
        if (field->tail) printf(", ");
        field = field->tail;
    }
    printf(")");
}

void printSymbolTable() {
    printf("\n========== SYMBOL TABLE ==========\n");
    bool empty = true;
    for (int i = 0; i < HASH_TABLE_SIZE; ++i) {
        Symbol* sym = hashTable[i];
        while (sym) {
            empty = false;
            printf("[Bucket %d] Name: %-20s Kind: ", i, sym->name);
            switch (sym->kind) {
                case SYMBOL_VARIABLE:
                    printf("VARIABLE  Type: ");
                    printType(sym->info.variableType);
                    break;
                case SYMBOL_FUNCTION:
                    printf("FUNCTION  Return: ");
                    printType(sym->info.funcInfo.returnType);
                    printf("  Params: %d ", sym->info.funcInfo.paramCount);
                    printFieldList(sym->info.funcInfo.params);
                    break;
                case SYMBOL_STRUCT:
                    printf("STRUCT    Type: ");
                    printType(sym->info.structType);
                    break;
            }
            printf("  Depth: %d\n", sym->depth);
            sym = sym->next;
        }
    }
    if (empty) {
        printf("(empty)\n");
    }
    printf("==================================\n\n");
}

// ==================== Main Semantic Analysis Function ====================

// Helper to traverse extDefList with current pass setting
static void traverseExtDefList(TreeNode *extDefList) {
  while (extDefList && extDefList->type == NODE_EXTDEF_LIST) {
    TreeNode *extDef = extDefList->firstChild;
    if (extDef && extDef->type == NODE_EXTDEF) {
      analyzeExtDef(extDef);
    }
    // Next extDefList is the sibling of extDef
    extDefList = extDef ? extDef->nextSibling : NULL;
  }
}

void analyzeSemantics(TreeNode *root) {
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
    Symbol *symbol = hashTable[i];
    while (symbol) {
      Symbol *next = symbol->next;
      // Free type information if needed
      if (symbol->kind == SYMBOL_VARIABLE) {
        freeType(symbol->info.variableType);
      } else if (symbol->kind == SYMBOL_FUNCTION) {
        freeType(symbol->info.funcInfo.returnType);
        // Free parameters
        FieldList param = symbol->info.funcInfo.params;
        while (param) {
          FieldList nextParam = param->tail;

          // We don't free here, for it can be hold by several variable and
          // function freeType(param->type);
          free(param);
          param = nextParam;
        }
      } else if (symbol->kind == SYMBOL_STRUCT) {
        // For struct symbols, we need to free the field list specially
        if (symbol->info.structType) {
          if (symbol->info.structType->kind == TYPE_STRUCTURE) {
            freeFieldList(symbol->info.structType->u.structure);
            // Set to NULL to avoid double-free
            symbol->info.structType->u.structure = NULL;
          }
          freeType(symbol->info.structType);
        }
      }
      free(symbol);
      symbol = next;
    }
    hashTable[i] = NULL;
  }
}
