#ifndef TREE_H
#define TREE_H
// 语法树节点类型定义
typedef enum {
    NODE_PROGRAM, NODE_EXTDEF, NODE_EXTDEF_LIST, NODE_SPECIFIER,NODE_STRUCTSPECIFIER,
    NODE_OPTTAG, NODE_TAG, NODE_EXTDECLIST,
    NODE_VARDEC, NODE_FUNDEC, NODE_COMPST, NODE_STMTLIST, NODE_STMT,
    NODE_DEFLIST, NODE_DEF, NODE_DECLIST, NODE_DEC, NODE_EXPR, NODE_ARGS,
    NODE_EXPRLIST, NODE_PARAMS, NODE_PARAMDECLIST, NODE_PARAMDEC,
    NODE_TYPE, NODE_STRUCT, NODE_INT, NODE_FLOAT, NODE_ID, NODE_ASSIGNOP,
    NODE_RELOP, NODE_PLUS, NODE_MINUS, NODE_STAR, NODE_DIV, NODE_AND,
    NODE_OR, NODE_NOT, NODE_SEMI, NODE_COMMA, NODE_DOT, NODE_LP, NODE_RP,
    NODE_LB, NODE_RB, NODE_LC, NODE_RC, NODE_RETURN, NODE_IF, NODE_ELSE,
    NODE_WHILE
} NodeType;

extern int parse_ok;


// 语法树节点结构定义
typedef struct TreeNode {
    NodeType type;          // 节点类型
    char name[32];          // 节点名称（用于存储标识符、关键字等）
    int intVal;             // 存储整数值
    float floatVal;         // 存储浮点数值
    int lineNumber;         // 行号
    struct TreeNode* firstChild;  // 第一个子节点
    struct TreeNode* nextSibling; // 下一个兄弟节点
} TreeNode;

// 创建语法树节点的函数
// maintain a lineNumber for later
TreeNode* createTreeNode(NodeType type, char* name, int lineNumber) ;

// 向父节点添加子节点的函数
void addChild(TreeNode* parent, TreeNode* child); 


// 打印语法树的函数
void printTree(TreeNode* root, int level);


// 释放语法树内存的函数
void freeTree(TreeNode* root);

// Get node type name for debugging
const char* nodeTypeName(NodeType type);

#endif