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



