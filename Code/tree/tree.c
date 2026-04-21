#include"tree.h"
#include<stddef.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>



// 创建语法树节点的函数
// maintain a lineNumber for later
TreeNode* createTreeNode(NodeType type, char* name, int lineNumber) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    if (node == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    node->type = type;
    strncpy(node->name, name, 31);
    node->name[31] = '\0';
    node->intVal = 0;
    node->floatVal = 0.0;
    node->lineNumber = lineNumber;
    node->firstChild = NULL;
    node->nextSibling = NULL;
    return node;
}

// 向父节点添加子节点的函数
void addChild(TreeNode* parent, TreeNode* child) {
    // if null then we don't do this 
    if (parent == NULL || child == NULL) {
        return;
    }

    // if the first child is null, then we let child as the fisrt
    if (parent->firstChild == NULL) {
        parent->firstChild = child;
    } else {
        TreeNode* sibling = parent->firstChild;
        // add it for tree
        while (sibling->nextSibling != NULL) {
            sibling = sibling->nextSibling;
        }
        sibling->nextSibling = child;
    }
}

// 打印语法树的函数
void printTree(TreeNode* root, int level) {
    // if error, no need to print tree;
    if(!parse_ok)
    {
        return;
    }

    if (root == NULL) {
        return;
    }

    // 打印缩进
    for (int i = 0; i < level; i++) {
        printf("  ");
    }

    // 区分语法单元和词法单元
    // 语法单元：需要打印名称和行号
    // 词法单元：只需打印名称，某些需要额外信息
    switch (root->type) {
        // 词法单元
        case NODE_TYPE:
            printf("TYPE: %s\n", root->name);
            break;
        case NODE_INT:
            printf("INT: %d\n", root->intVal);
            break;
        case NODE_FLOAT:
            printf("FLOAT: %.6f\n", root->floatVal);
            break;
        case NODE_ID:
            printf("ID: %s\n", root->name);
            break;
        case NODE_ASSIGNOP:
            printf("ASSIGNOP\n");
            break;
        case NODE_RELOP:
            printf("RELOP\n");
            break;
        case NODE_PLUS:
            printf("PLUS\n");
            break;
        case NODE_MINUS:
            printf("MINUS\n");
            break;
        case NODE_STAR:
            printf("STAR\n");
            break;
        case NODE_DIV:
            printf("DIV\n");
            break;
        case NODE_AND:
            printf("AND\n");
            break;
        case NODE_OR:
            printf("OR\n");
            break;
        case NODE_NOT:
            printf("NOT\n");
            break;
        case NODE_SEMI:
            printf("SEMI\n");
            break;
        case NODE_COMMA:
            printf("COMMA\n");
            break;
        case NODE_DOT:
            printf("DOT\n");
            break;
        case NODE_LP:
            printf("LP\n");
            break;
        case NODE_RP:
            printf("RP\n");
            break;
        case NODE_LB:
            printf("LB\n");
            break;
        case NODE_RB:
            printf("RB\n");
            break;
        case NODE_LC:
            printf("LC\n");
            break;
        case NODE_RC:
            printf("RC\n");
            break;
        case NODE_STRUCT:
            printf("STRUCT\n");
            break;
        case NODE_RETURN:
            printf("RETURN\n");
            break;
        case NODE_IF:
            printf("IF\n");
            break;
        case NODE_ELSE:
            printf("ELSE\n");
            break;
        case NODE_WHILE:
            printf("WHILE\n");
            break;

        // 语法单元
        default:
            // print name first then come the lineNumber
            printf("%s (%d)\n", root->name, root->lineNumber);
            break;
    }

    // 递归打印子节点
    printTree(root->firstChild, level + 1);

    // 打印兄弟节点
    printTree(root->nextSibling, level);
}

// 释放语法树内存的函数
void freeTree(TreeNode* root) {
    if (root == NULL) {
        return;
    }

    freeTree(root->firstChild);
    freeTree(root->nextSibling);
    free(root);
}

// Get node type name for debugging
const char* nodeTypeName(NodeType type) {
    switch (type) {
        case NODE_PROGRAM: return "NODE_PROGRAM";
        case NODE_EXTDEF: return "NODE_EXTDEF";
        case NODE_EXTDEF_LIST: return "NODE_EXTDEF_LIST";
        case NODE_SPECIFIER: return "NODE_SPECIFIER";
        case NODE_STRUCTSPECIFIER: return "NODE_STRUCTSPECIFIER";
        case NODE_OPTTAG: return "NODE_OPTTAG";
        case NODE_TAG: return "NODE_TAG";
        case NODE_EXTDECLIST: return "NODE_EXTDECLIST";
        case NODE_VARDEC: return "NODE_VARDEC";
        case NODE_FUNDEC: return "NODE_FUNDEC";
        case NODE_COMPST: return "NODE_COMPST";
        case NODE_STMTLIST: return "NODE_STMTLIST";
        case NODE_STMT: return "NODE_STMT";
        case NODE_DEFLIST: return "NODE_DEFLIST";
        case NODE_DEF: return "NODE_DEF";
        case NODE_DECLIST: return "NODE_DECLIST";
        case NODE_DEC: return "NODE_DEC";
        case NODE_EXPR: return "NODE_EXPR";
        case NODE_ARGS: return "NODE_ARGS";
        case NODE_EXPRLIST: return "NODE_EXPRLIST";
        case NODE_PARAMS: return "NODE_PARAMS";
        case NODE_PARAMDECLIST: return "NODE_PARAMDECLIST";
        case NODE_PARAMDEC: return "NODE_PARAMDEC";
        case NODE_TYPE: return "NODE_TYPE";
        case NODE_STRUCT: return "NODE_STRUCT";
        case NODE_INT: return "NODE_INT";
        case NODE_FLOAT: return "NODE_FLOAT";
        case NODE_ID: return "NODE_ID";
        case NODE_ASSIGNOP: return "NODE_ASSIGNOP";
        case NODE_RELOP: return "NODE_RELOP";
        case NODE_PLUS: return "NODE_PLUS";
        case NODE_MINUS: return "NODE_MINUS";
        case NODE_STAR: return "NODE_STAR";
        case NODE_DIV: return "NODE_DIV";
        case NODE_AND: return "NODE_AND";
        case NODE_OR: return "NODE_OR";
        case NODE_NOT: return "NODE_NOT";
        case NODE_SEMI: return "NODE_SEMI";
        case NODE_COMMA: return "NODE_COMMA";
        case NODE_DOT: return "NODE_DOT";
        case NODE_LP: return "NODE_LP";
        case NODE_RP: return "NODE_RP";
        case NODE_LB: return "NODE_LB";
        case NODE_RB: return "NODE_RB";
        case NODE_LC: return "NODE_LC";
        case NODE_RC: return "NODE_RC";
        case NODE_RETURN: return "NODE_RETURN";
        case NODE_IF: return "NODE_IF";
        case NODE_ELSE: return "NODE_ELSE";
        case NODE_WHILE: return "NODE_WHILE";
        default: return "UNKNOWN_NODE_TYPE";
    }
}



