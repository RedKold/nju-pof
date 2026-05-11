#include "translate.h"
#include "tree/tree.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

// ==================== Global Variables ====================

static IRList_ ir_list;

// ==================== Forward Declarations ====================

static void translateExtDef(TreeNode *node);
static void traverseExtDefList(TreeNode *extDefList);
static void translateCompSt(TreeNode *node);
static void translateDef(TreeNode *node);
static void traverseDefList(TreeNode *defList);
static void traverseStmtList(TreeNode *stmtList);
static void processVarDec(TreeNode *varDecNode);

// ==================== Initialization & Cleanup ====================

void initTranslate() {
    ir_list.head = NULL;
    ir_list.tail = NULL;
    ir_list.temp_count = 0;
    ir_list.label_count = 0;
}

void cleanupTranslate() {
    IRInstruction curr = ir_list.head;
    while (curr != NULL) {
        IRInstruction next = curr->next;
        free(curr);
        curr = next;
    }
    ir_list.head = NULL;
    ir_list.tail = NULL;
}

// ==================== Helper Functions ====================

char* new_temp() {
    ir_list.temp_count++;
    char* temp = (char*)malloc(32);
    snprintf(temp, 32, "t%d", ir_list.temp_count);
    return temp;
}

char* new_label() {
    ir_list.label_count++;
    char* label = (char*)malloc(32);
    snprintf(label, 32, "label%d", ir_list.label_count);
    return label;
}

IRInstruction createIR(IRType type) {
    IRInstruction inst = (IRInstruction)malloc(sizeof(IRInstruction_));
    if (inst == NULL) {
        fprintf(stderr, "Out of memory when creating IR instruction\n");
        exit(1);
    }
    inst->type = type;
    inst->prev = NULL;
    inst->next = NULL;
    return inst;
}

void addIR(IRInstruction inst) {
    if (ir_list.tail == NULL) {
        ir_list.head = inst;
        ir_list.tail = inst;
    } else {
        ir_list.tail->next = inst;
        inst->prev = ir_list.tail;
        ir_list.tail = inst;
    }
}

// Helper: Add assign instruction, skipping self-assign
void addAssignIR(const char* left, const char* right) {
    if (strcmp(left, right) == 0) {
        return;  // Skip self-assignment
    }
    IRInstruction inst = createIR(IR_ASSIGN);
    strncpy(inst->u.assign.left, left, 31);
    inst->u.assign.left[31] = '\0';
    strncpy(inst->u.assign.right, right, 31);
    inst->u.assign.right[31] = '\0';
    addIR(inst);
}

// Helper: Get relop string for printing
const char* relopToString(RelOp op) {
    switch (op) {
        case RELOP_LT: return "<";
        case RELOP_LE: return "<=";
        case RELOP_GT: return ">";
        case RELOP_GE: return ">=";
        case RELOP_EQ: return "==";
        case RELOP_NE: return "!=";
        default: return "??";
    }
}

// Helper: Get binop string for printing
const char* binopToString(BinOp op) {
    switch (op) {
        case OP_PLUS: return "+";
        case OP_MINUS: return "-";
        case OP_MUL: return "*";
        case OP_DIV: return "/";
        default: return "??";
    }
}

void printIR(FILE* out) {
    IRInstruction curr = ir_list.head;
    while (curr != NULL) {
        switch (curr->type) {
            case IR_LABEL:
                fprintf(out, "LABEL %s:\n", curr->u.label.label);
                break;
            case IR_FUNCTION:
                fprintf(out, "FUNCTION %s:\n", curr->u.func.func);
                break;
            case IR_ASSIGN:
                fprintf(out, "%s := %s\n", curr->u.assign.left, curr->u.assign.right);
                break;
            case IR_BINOP:
                fprintf(out, "%s := %s %s %s\n",
                    curr->u.binop.result,
                    curr->u.binop.arg1,
                    binopToString(curr->u.binop.op),
                    curr->u.binop.arg2);
                break;
            case IR_UNIOP:
                fprintf(out, "%s := #0 - %s\n", curr->u.uniop.result, curr->u.uniop.arg);
                break;
            case IR_ADDR:
                fprintf(out, "%s := &%s\n", curr->u.addr.left, curr->u.addr.right);
                break;
            case IR_DEREF_L:
                fprintf(out, "%s := *%s\n", curr->u.deref_l.left, curr->u.deref_l.right);
                break;
            case IR_DEREF_R:
                fprintf(out, "*%s := %s\n", curr->u.deref_r.left, curr->u.deref_r.right);
                break;
            case IR_GOTO:
                fprintf(out, "GOTO %s\n", curr->u.goto_.label);
                break;
            case IR_IF_GOTO:
                fprintf(out, "IF %s %s %s GOTO %s\n",
                    curr->u.if_goto.arg1,
                    relopToString(curr->u.if_goto.op),
                    curr->u.if_goto.arg2,
                    curr->u.if_goto.label);
                break;
            case IR_RETURN:
                fprintf(out, "RETURN %s\n", curr->u.return_.val);
                break;
            case IR_DEC:
                fprintf(out, "DEC %s %d\n", curr->u.dec.var, curr->u.dec.size);
                break;
            case IR_ARG:
                fprintf(out, "ARG %s\n", curr->u.arg.arg);
                break;
            case IR_CALL:
                fprintf(out, "%s := CALL %s\n", curr->u.call.result, curr->u.call.func);
                break;
            case IR_PARAM:
                fprintf(out, "PARAM %s\n", curr->u.param.param);
                break;
            case IR_READ:
                fprintf(out, "READ %s\n", curr->u.read.var);
                break;
            case IR_WRITE:
                fprintf(out, "WRITE %s\n", curr->u.write.var);
                break;
        }
        curr = curr->next;
    }
}

// ==================== Main Translation Functions ====================

// Helper to traverse extDefList
static void traverseExtDefList(TreeNode *extDefList) {
    while (extDefList && extDefList->type == NODE_EXTDEF_LIST) {
        TreeNode *extDef = extDefList->firstChild;
        if (extDef && extDef->type == NODE_EXTDEF) {
            translateExtDef(extDef);
        }
        // Next extDefList is the sibling of extDef
        extDefList = extDef ? extDef->nextSibling : NULL;
    }
}

// Helper to traverse defList
static void traverseDefList(TreeNode *defList) {
    while (defList && defList->type == NODE_DEFLIST) {
        TreeNode *def = defList->firstChild;
        if (def && def->type == NODE_DEF) {
            translateDef(def);
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
            translateStmt(stmt);
        }
        // Next stmtList is the sibling of stmt
        stmtList = stmt ? stmt->nextSibling : NULL;
    }
}

// Helper to extract identifier name from a node (ID or VarDec)
static const char* getIdentifierName(TreeNode* node) {
    // only can return ID
    if (!node) {
      return NULL;
    }
    if (node->type == NODE_ID) return node->name;
    // If it's a VarDec, recursively look for the ID child
    if (node->type == NODE_VARDEC) {
        return getIdentifierName(node->firstChild);
    }
    return NULL;
}

// Translate an ExtDef node (external definition: global var, struct, function)
static void translateExtDef(TreeNode *node) {
    if (!node || node->type != NODE_EXTDEF) {
        return;
    }

    TreeNode *specifier = node->firstChild;
    if (!specifier) {
        return;
    }

    TreeNode *next = specifier->nextSibling;
    if (!next) {
        // Just a specifier (like struct definition without variables)
        return;
    }

    if (next->type == NODE_FUNDEC) {
        // Function definition
        translateFuncDef(node);
    }
    // For basic requirements, we ignore global variables
}

// Translate a Def node (local variable declaration)
static void translateDef(TreeNode *node) {
    if (!node || node->type != NODE_DEF) {
        return;
    }

    TreeNode *specifier = node->firstChild;
    if (!specifier) {
        return;
    }

    TreeNode *decList = specifier->nextSibling;
    if (!decList || decList->type != NODE_DECLIST) {
        return;
    }

    // Process each declaration in decList
    TreeNode *dec = decList->firstChild;
    while (dec) {
        if (dec->type == NODE_DEC) {
            TreeNode *varDec = dec->firstChild;
            if (varDec && varDec->type == NODE_VARDEC) {
                processVarDec(varDec);
            }
        }
        dec = dec->nextSibling;
    }
}

// Process a VarDec node - generate DEC instructions for arrays/structs
static void processVarDec(TreeNode *varDecNode) {
    if (!varDecNode || varDecNode->type != NODE_VARDEC) {
        return;
    }

    // TODO: For basic requirements, we only need to handle simple variables
    // For arrays and structs, we need to generate DEC instructions
}

// Translate a CompSt node (compound statement)
static void translateCompSt(TreeNode *node) {
    if (!node || node->type != NODE_COMPST) {
        return;
    }

    TreeNode *child = node->firstChild; // should be LC
    if (child) {
        child = child->nextSibling; // DefList (optional)
    }

    // Translate definitions (variable declarations)
    if (child && child->type == NODE_DEFLIST) {
        traverseDefList(child);
        child = child->nextSibling; // move to StmtList
    }

    // Translate statements
    if (child && child->type == NODE_STMTLIST) {
        traverseStmtList(child);
    }
}

void translateProgram(TreeNode* root, const char* outputFilename) {
    if (root == NULL || root->type != NODE_PROGRAM) {
        fprintf(stderr, "Invalid root node for IR generation\n");
        return;
    }

    initTranslate();

    // Traverse the AST and generate IR
    traverseExtDefList(root->firstChild);

    // Output the generated IR
    FILE* out = fopen(outputFilename, "w");
    if (out != NULL) {
        printIR(out);
        fclose(out);
    }

    // Clean up malloc'd labels and temps would require more complex tracking
    // For now, we just clean up instructions
    cleanupTranslate();
}

void translateFuncDef(TreeNode* node) {
    if (node == NULL || node->type != NODE_EXTDEF) return;

    TreeNode *specifier = node->firstChild;
    if (!specifier) return;

    TreeNode *funDec = specifier->nextSibling;
    if (!funDec || funDec->type != NODE_FUNDEC) return;

    TreeNode *idNode = funDec->firstChild;
    const char *funcName = getIdentifierName(idNode);
    if (!funcName) return;

    // 2. Generate IR_FUNCTION instruction
    IRInstruction funcInst = createIR(IR_FUNCTION);
    strncpy(funcInst->u.func.func, funcName, 31);
    funcInst->u.func.func[31] = '\0';
    addIR(funcInst);

    // 3. Generate IR_PARAM for each parameter
    TreeNode *lp = idNode ? idNode->nextSibling : NULL; // (
    if (lp && lp->type == NODE_LP) {
        TreeNode *paramList = lp->nextSibling; // VarList or RP
        while (paramList && paramList->type != NODE_RP) {
            if (paramList->type == NODE_PARAMDEC) {
                TreeNode *paramSpec = paramList->firstChild;
                TreeNode *paramVarDec = paramSpec ? paramSpec->nextSibling : NULL;
                if (paramVarDec) {
                    const char *paramName = getIdentifierName(paramVarDec);
                    if (paramName) {
                        IRInstruction paramInst = createIR(IR_PARAM);
                        strncpy(paramInst->u.param.param, paramName, 31);
                        paramInst->u.param.param[31] = '\0';
                        addIR(paramInst);
                    }
                }
            }
            paramList = paramList->nextSibling;
        }
    }

    // 4. Translate the compound statement body
    TreeNode *compSt = funDec->nextSibling;
    if (compSt && compSt->type == NODE_COMPST) {
        translateCompSt(compSt);
    }
}

void translateStmt(TreeNode* node) {
    if (node == NULL || node->type != NODE_STMT) return;

    TreeNode *child = node->firstChild;
    if (!child) return;

    switch (child->type) {
        case NODE_RETURN: {
            // Return statement
            TreeNode *expr = child->nextSibling;
            if (expr && expr->type == NODE_EXPR) {
                char *temp = new_temp();
                translateExp(expr, temp);
                IRInstruction retInst = createIR(IR_RETURN);
                strncpy(retInst->u.return_.val, temp, 31);
                retInst->u.return_.val[31] = '\0';
                addIR(retInst);
                free(temp);
            } else {
                // Return without value
                IRInstruction retInst = createIR(IR_RETURN);
                retInst->u.return_.val[0] = '\0';
                addIR(retInst);
            }
            break;
        }
        case NODE_COMPST:
            // Compound statement
            translateCompSt(child);
            break;
        case NODE_IF: {
            // If statement: IF LP EXPR RP STMT (ELSE STMT)?
            TreeNode* lp = child->nextSibling;          // LP
            TreeNode* expr = lp ? lp->nextSibling : NULL;    // EXPR
            TreeNode* rp = expr ? expr->nextSibling : NULL;  // RP
            TreeNode* stmt1 = rp ? rp->nextSibling : NULL;  // STMT

            // 检查是否有 else 分支
            TreeNode* else_node = NULL;
            TreeNode* stmt2 = NULL;
            if (stmt1 && stmt1->nextSibling && stmt1->nextSibling->type == NODE_ELSE) {
                else_node = stmt1->nextSibling;
                stmt2 = else_node->nextSibling;
            }

            char* label_true = new_label();
            char* label_false = new_label();
            char* label_end = new_label();


            // 翻译条件
            translateCond(expr, label_true, label_false);

            // true 分支
            IRInstruction label_true_inst = createIR(IR_LABEL);
            strncpy(label_true_inst->u.label.label, label_true, 31);
            label_true_inst->u.label.label[31] = '\0';
            addIR(label_true_inst);

            translateStmt(stmt1);

            if (else_node) {
                // 如果有 else，跳转到结束
                IRInstruction goto_end = createIR(IR_GOTO);
                strncpy(goto_end->u.goto_.label, label_end, 31);
                goto_end->u.goto_.label[31] = '\0';
                addIR(goto_end);
            }

            // false 分支
            IRInstruction label_false_inst = createIR(IR_LABEL);
            strncpy(label_false_inst->u.label.label, label_false, 31);
            label_false_inst->u.label.label[31] = '\0';
            addIR(label_false_inst);

            if (else_node && stmt2) {
                translateStmt(stmt2);
            }

            // 结束标签（仅在有 else 时需要）
            if (else_node) {
                IRInstruction label_end_inst = createIR(IR_LABEL);
                strncpy(label_end_inst->u.label.label, label_end, 31);
                label_end_inst->u.label.label[31] = '\0';
                addIR(label_end_inst);
            }

            free(label_true);
            free(label_false);
            free(label_end);
            break;
        }
        case NODE_WHILE: {
            // While statement: WHILE LP EXPR RP STMT
            TreeNode* lp = child->nextSibling;          // LP
            TreeNode* expr = lp ? lp->nextSibling : NULL;    // EXPR
            TreeNode* rp = expr ? expr->nextSibling : NULL;  // RP
            TreeNode* stmt = rp ? rp->nextSibling : NULL;   // STMT

            char* label_begin = new_label();
            char* label_true = new_label();
            char* label_false = new_label();

            // 循环开始标签
            IRInstruction label_begin_inst = createIR(IR_LABEL);
            strncpy(label_begin_inst->u.label.label, label_begin, 31);
            label_begin_inst->u.label.label[31] = '\0';
            addIR(label_begin_inst);

            // 翻译条件
            translateCond(expr, label_true, label_false);

            // 循环体
            IRInstruction label_true_inst = createIR(IR_LABEL);
            strncpy(label_true_inst->u.label.label, label_true, 31);
            label_true_inst->u.label.label[31] = '\0';
            addIR(label_true_inst);

            translateStmt(stmt);

            // 回到循环开始
            IRInstruction goto_begin = createIR(IR_GOTO);
            strncpy(goto_begin->u.goto_.label, label_begin, 31);
            goto_begin->u.goto_.label[31] = '\0';
            addIR(goto_begin);

            // 循环结束
            IRInstruction label_false_inst = createIR(IR_LABEL);
            strncpy(label_false_inst->u.label.label, label_false, 31);
            label_false_inst->u.label.label[31] = '\0';
            addIR(label_false_inst);

            free(label_begin);
            free(label_true);
            free(label_false);
            break;
        }
        default:
            // Expression statement - 不需要保存结果，直接翻译，避免无用赋值
            if (child->type == NODE_EXPR) {
                // 检查是否是函数调用，如果是，直接处理
                TreeNode* exp_child = child->firstChild;
                if (exp_child && exp_child->type == NODE_ID && exp_child->nextSibling &&
                    exp_child->nextSibling->type == NODE_LP) {
                    // 是函数调用，直接翻译，不保存返回值（除非是 read 等）
                    // 但为了简化，我们还是用一个临时变量，但不生成最后的赋值
                    char *temp = new_temp();
                    translateExp(child, temp);
                    free(temp);
                } else {
                    // 其他表达式语句，正常处理
                    char *temp = new_temp();
                    translateExp(child, temp);
                    free(temp);
                }
            }
            break;
    }
}

void translateExp(TreeNode* node, char* place) {
    if (node == NULL || place == NULL) return;

    if (node->type != NODE_EXPR) {
        // 如果不是表达式节点，可能是 ID 或 INT，直接处理
        if (node->type == NODE_INT) {
            // 整数字面量
            char value_str[32];
            snprintf(value_str, sizeof(value_str), "#%d", node->intVal);
            addAssignIR(place, value_str);
        } else if (node->type == NODE_ID) {
            addAssignIR(place, node->name);
        }
        return;
    }

    // 解析表达式节点的子节点结构
    TreeNode* child = node->firstChild;
    if (child == NULL) return;

    // 情况1: 单个子节点 (INT, ID, 或括号表达式)
    if (child->nextSibling == NULL) {
        translateExp(child, place);
        return;
    }

    // 情况2: 一元负号: -E
    if (child->type == NODE_MINUS) {
        TreeNode* operand = child->nextSibling;
        char* temp = new_temp();
        translateExp(operand, temp);

        IRInstruction inst = createIR(IR_UNIOP);
        strncpy(inst->u.uniop.result, place, 31);
        inst->u.uniop.result[31] = '\0';
        strncpy(inst->u.uniop.arg, temp, 31);
        inst->u.uniop.arg[31] = '\0';
        addIR(inst);

        free(temp);
        return;
    }

    // 情况3: 括号表达式: (E)
    if (child->type == NODE_LP) {
        TreeNode* expr = child->nextSibling;
        if (expr && expr->type == NODE_EXPR) {
            translateExp(expr, place);
        }
        return;
    }

    // 情况4: 函数调用: ID LP ...
    if (child->type == NODE_ID && child->nextSibling != NULL && child->nextSibling->type == NODE_LP) {
        TreeNode* lp = child->nextSibling;
        TreeNode* args_node = lp->nextSibling;

        // 先处理参数
        char* arg_list[16];  // 最多支持16个参数
        int arg_count = 0;

        if (args_node && args_node->type == NODE_ARGS) {
            // 有参数
            translateArgs(args_node, arg_list, &arg_count);
        }

        // 生成 ARG 指令（反向传递）
        for (int i = arg_count - 1; i >= 0; i--) {
            IRInstruction arg_inst = createIR(IR_ARG);
            strncpy(arg_inst->u.arg.arg, arg_list[i], 31);
            arg_inst->u.arg.arg[31] = '\0';
            addIR(arg_inst);
        }

        // 检查是否是 read 或 write 特殊函数
        if (strcmp(child->name, "read") == 0) {
            IRInstruction read_inst = createIR(IR_READ);
            strncpy(read_inst->u.read.var, place, 31);
            read_inst->u.read.var[31] = '\0';
            addIR(read_inst);
        } else if (strcmp(child->name, "write") == 0) {
            // write 只有一个参数
            if (arg_count >= 1) {
                IRInstruction write_inst = createIR(IR_WRITE);
                strncpy(write_inst->u.write.var, arg_list[0], 31);
                write_inst->u.write.var[31] = '\0';
                addIR(write_inst);
                // write 也返回值，所以赋值（只在不是自赋值时）
                addAssignIR(place, arg_list[0]);
            }
        } else {
            // 普通函数调用
            IRInstruction call_inst = createIR(IR_CALL);
            strncpy(call_inst->u.call.result, place, 31);
            call_inst->u.call.result[31] = '\0';
            strncpy(call_inst->u.call.func, child->name, 31);
            call_inst->u.call.func[31] = '\0';
            addIR(call_inst);
        }

        // 释放参数 temp
        for (int i = 0; i < arg_count; i++) {
            free(arg_list[i]);
        }
        return;
    }

    // 情况5: 二元运算或赋值 (左操作数, 操作符, 右操作数)
    // 子节点结构：left -> op -> right
    TreeNode* left = child;
    TreeNode* op_node = left->nextSibling;
    TreeNode* right = op_node ? op_node->nextSibling : NULL;

    if (op_node && right) {
        // 赋值表达式
        if (op_node->type == NODE_ASSIGNOP) {
            // 先翻译右边的表达式
            char* temp_right = new_temp();
            translateExp(right, temp_right);


            assert(left);

            // here you should get the right n!
            const char* left_name = getIdentifierName(left->firstChild);

            printf("left_name is %s\n", left_name);

            if (left_name) {
                addAssignIR((char*)left_name, temp_right);
                // 赋值表达式也返回值
                addAssignIR(place, temp_right);
            } else {
                // 如果 left 不是简单标识符（比如数组访问或结构体成员）
                // 这里我们先不处理，因为基本需求不包含
                addAssignIR(place, temp_right);
            }

            free(temp_right);
        }
        // 二元算术运算
        else if (op_node->type == NODE_PLUS || op_node->type == NODE_MINUS ||
                 op_node->type == NODE_STAR || op_node->type == NODE_DIV) {
            char* temp_left = new_temp();
            char* temp_right = new_temp();

            translateExp(left, temp_left);
            translateExp(right, temp_right);

            BinOp op;
            if (op_node->type == NODE_PLUS) op = OP_PLUS;
            else if (op_node->type == NODE_MINUS) op = OP_MINUS;
            else if (op_node->type == NODE_STAR) op = OP_MUL;
            else op = OP_DIV;

            IRInstruction inst = createIR(IR_BINOP);
            strncpy(inst->u.binop.result, place, 31);
            inst->u.binop.result[31] = '\0';
            strncpy(inst->u.binop.arg1, temp_left, 31);
            inst->u.binop.arg1[31] = '\0';
            inst->u.binop.op = op;
            strncpy(inst->u.binop.arg2, temp_right, 31);
            inst->u.binop.arg2[31] = '\0';
            addIR(inst);

            free(temp_left);
            free(temp_right);
        }
    }
}

// Helper to parse the relop string from the T_RELOP node
static RelOp parse_relop(TreeNode* node) {
    if (!node) return RELOP_EQ;
    if (strcmp(node->name, "<") == 0) return RELOP_LT;
    if (strcmp(node->name, "<=") == 0) return RELOP_LE;
    if (strcmp(node->name, ">") == 0) return RELOP_GT;
    if (strcmp(node->name, ">=") == 0) return RELOP_GE;
    if (strcmp(node->name, "==") == 0) return RELOP_EQ;
    if (strcmp(node->name, "!=") == 0) return RELOP_NE;
    return RELOP_EQ;
}

void translateCond(TreeNode* node, char* label_true, char* label_false) {
    if (node == NULL || label_true == NULL || label_false == NULL) return;

    // If it's an expression node, check what kind it is
    if (node->type == NODE_EXPR) {
        TreeNode* left = node->firstChild;
        if (!left) return;
        TreeNode* op_node = left->nextSibling;
        if (!op_node) {
            // Single child - just check if non-zero
            char* temp = new_temp();
            translateExp(node, temp);
            IRInstruction inst1 = createIR(IR_IF_GOTO);
            strncpy(inst1->u.if_goto.arg1, temp, 31);
            inst1->u.if_goto.arg1[31] = '\0';
            inst1->u.if_goto.op = RELOP_NE;
            strncpy(inst1->u.if_goto.arg2, "#0", 31);
            inst1->u.if_goto.arg2[31] = '\0';
            strncpy(inst1->u.if_goto.label, label_true, 31);
            inst1->u.if_goto.label[31] = '\0';
            addIR(inst1);
            IRInstruction inst2 = createIR(IR_GOTO);
            strncpy(inst2->u.goto_.label, label_false, 31);
            inst2->u.goto_.label[31] = '\0';
            addIR(inst2);
            free(temp);
            return;
        }
        TreeNode* right = op_node->nextSibling;
        if (!right) return;

        if (op_node->type == NODE_RELOP) {
            // Relational operator: left relop right
            char* temp_left = new_temp();
            char* temp_right = new_temp();
            translateExp(left, temp_left);
            translateExp(right, temp_right);
            RelOp op = parse_relop(op_node);
            IRInstruction inst1 = createIR(IR_IF_GOTO);
            strncpy(inst1->u.if_goto.arg1, temp_left, 31);
            inst1->u.if_goto.arg1[31] = '\0';
            inst1->u.if_goto.op = op;
            strncpy(inst1->u.if_goto.arg2, temp_right, 31);
            inst1->u.if_goto.arg2[31] = '\0';
            strncpy(inst1->u.if_goto.label, label_true, 31);
            inst1->u.if_goto.label[31] = '\0';
            addIR(inst1);
            IRInstruction inst2 = createIR(IR_GOTO);
            strncpy(inst2->u.goto_.label, label_false, 31);
            inst2->u.goto_.label[31] = '\0';
            addIR(inst2);
            free(temp_left);
            free(temp_right);
        } else if (op_node->type == NODE_AND) {
            // Logical AND: left && right (short-circuit)
            char* mid_label = new_label();
            translateCond(left, mid_label, label_false);
            IRInstruction mid_inst = createIR(IR_LABEL);
            strncpy(mid_inst->u.label.label, mid_label, 31);
            mid_inst->u.label.label[31] = '\0';
            addIR(mid_inst);
            translateCond(right, label_true, label_false);
            free(mid_label);
        } else if (op_node->type == NODE_OR) {
            // Logical OR: left || right (short-circuit)
            char* mid_label = new_label();
            translateCond(left, label_true, mid_label);
            IRInstruction mid_inst = createIR(IR_LABEL);
            strncpy(mid_inst->u.label.label, mid_label, 31);
            mid_inst->u.label.label[31] = '\0';
            addIR(mid_inst);
            translateCond(right, label_true, label_false);
            free(mid_label);
        } else {
            // Other binary operators - just evaluate and check non-zero
            char* temp = new_temp();
            translateExp(node, temp);
            IRInstruction inst1 = createIR(IR_IF_GOTO);
            strncpy(inst1->u.if_goto.arg1, temp, 31);
            inst1->u.if_goto.arg1[31] = '\0';
            inst1->u.if_goto.op = RELOP_NE;
            strncpy(inst1->u.if_goto.arg2, "#0", 31);
            inst1->u.if_goto.arg2[31] = '\0';
            strncpy(inst1->u.if_goto.label, label_true, 31);
            inst1->u.if_goto.label[31] = '\0';
            addIR(inst1);
            IRInstruction inst2 = createIR(IR_GOTO);
            strncpy(inst2->u.goto_.label, label_false, 31);
            inst2->u.goto_.label[31] = '\0';
            addIR(inst2);
            free(temp);
        }
    } else {
        // Single node (ID, INT, etc.) - check if non-zero
        // Also handle NOT here - check if this is a NOT expression
        TreeNode* child = node->firstChild;
        if (child && child->type == NODE_NOT) {
            // Logical NOT: !expr
            TreeNode* expr = child->nextSibling;
            if (expr) {
                translateCond(expr, label_false, label_true);
                return;
            }
        }
        // Otherwise, check non-zero
        char* temp = new_temp();
        translateExp(node, temp);
        IRInstruction inst1 = createIR(IR_IF_GOTO);
        strncpy(inst1->u.if_goto.arg1, temp, 31);
        inst1->u.if_goto.arg1[31] = '\0';
        inst1->u.if_goto.op = RELOP_NE;
        strncpy(inst1->u.if_goto.arg2, "#0", 31);
        inst1->u.if_goto.arg2[31] = '\0';
        strncpy(inst1->u.if_goto.label, label_true, 31);
        inst1->u.if_goto.label[31] = '\0';
        addIR(inst1);
        IRInstruction inst2 = createIR(IR_GOTO);
        strncpy(inst2->u.goto_.label, label_false, 31);
        inst2->u.goto_.label[31] = '\0';
        addIR(inst2);
        free(temp);
    }
}

void translateArgs(TreeNode* node, char** arg_list, int* arg_count) {
    if (node == NULL || arg_list == NULL || arg_count == NULL) {
        *arg_count = 0;
        return;
    }

    *arg_count = 0;

    // 遍历参数列表
    TreeNode* arg = node->firstChild;
    while (arg && *arg_count < 16) {
        if (arg->type == NODE_EXPR) {
            // 翻译表达式
            char* temp = new_temp();
            translateExp(arg, temp);
            arg_list[*arg_count] = temp;
            (*arg_count)++;
        } else if (arg->type != NODE_COMMA) {
            // 如果是表达式（不是逗号）
            char* temp = new_temp();
            translateExp(arg, temp);
            arg_list[*arg_count] = temp;
            (*arg_count)++;
        }
        arg = arg->nextSibling;
    }
}
