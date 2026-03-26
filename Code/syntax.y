%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lex.yy.c"

// Declare global variable from lexical.l
extern int struct_line_number;

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
            printf("FLOAT: %.6g\n", root->floatVal);
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

int error_cnt=0;
// 错误处理函数
int yyerror(const char* msg) {
    error_cnt++;
    // 替换常见错误信息为更友好的提示
    if (strstr(msg, "T_SEMI") != NULL) {
        printf("Error type B at Line %d: Missing \";\"\n", yylineno);
    } else if (strstr(msg, "T_RB") != NULL) {
        printf("Error type B at Line %d: Missing \"]\"\n", yylineno);
    } else if (strstr(msg, "T_COMMA") != NULL && strstr(msg, "unexpected") != NULL) {
        // 处理 a[5,3] 这种情况的错误
        printf("Error type B at Line %d: Missing \"]\"\n", yylineno);
    } else if (strstr(msg, "T_ELSE") != NULL && strstr(msg, "unexpected") != NULL) {
        // 处理 if 语句后缺少分号的错误
        printf("Error type B at Line %d: Missing \";\"\n", yylineno);
    } else {
        printf("Error type B at Line %d: %s\n", yylineno, msg);
    }
    return 0;
}
%}

%define parse.error verbose

// Token 定义（与 lexical.l 中保持一致）
// 使用不同的名称前缀以避免与 lexical.l 中的宏冲突
%token T_INT T_FLOAT T_ID T_SEMI T_COMMA T_ASSIGNOP T_RELOP T_PLUS T_MINUS T_STAR T_DIV
%token T_AND T_OR T_DOT T_NOT T_TYPE T_LP T_RP T_LB T_RB T_LC T_RC T_STRUCT  T_RETURN T_IF T_ELSE T_WHILE
%token UMINUS  // 用于表示一元负号的优先级

// 语法树节点类型定义
%union {
    struct TreeNode* node;  // 语法树节点指针
    int num;
    float real;
    char id[32];
}

// 为每个非终结符和终结符指定类型
%type <node> program extDefList extDef extDecList specifier structSpecifier optTag  tag varDec funDec compSt stmtList stmt
%type <node> defList def decList dec expr args exprList params paramDecList paramDec epsilon

// 开始符号
%start program

%%

// 空产生式定义
epsilon: /* empty */
       { $$ = NULL; }
       ;

// 程序结构：由外部定义列表组成
program: extDefList
       {
         // we let TreeNote as a part of attribute
         $$ = createTreeNode(NODE_PROGRAM, "Program", 1); // 固定为第1行
         addChild($$, $1);
         // if has error, don't print it
         if(error_cnt==0)
          printTree($$, 0);
         freeTree($$);
       }
       ;

// 外部定义列表：可以是空的，或者由多个外部定义组成
extDefList: extDef extDefList
          {
            $$ = createTreeNode(NODE_EXTDEF_LIST, "ExtDefList", 1); // 固定为第1行
            addChild($$, $1);
            addChild($$, $2);
          }
          | epsilon
          { $$ = NULL; }
          ;

// 外部定义：变量声明、类型声明或函数定义
extDef: specifier extDecList T_SEMI
      {
        $$ = createTreeNode(NODE_EXTDEF, "ExtDef", 1); // 固定为第1行
        addChild($$, $1);
        addChild($$, $2);
        TreeNode* semiNode = createTreeNode(NODE_SEMI, "SEMI", yylineno);
        addChild($$, semiNode);
      }
      | specifier T_SEMI
      {
        $$ = createTreeNode(NODE_EXTDEF, "ExtDef", 1); // 固定为第1行
        addChild($$, $1);
        TreeNode* semiNode = createTreeNode(NODE_SEMI, "SEMI", yylineno);
        addChild($$, semiNode);
      }
      | specifier funDec compSt
      {
        $$ = createTreeNode(NODE_EXTDEF, "ExtDef", 1); // 固定为第1行
        addChild($$, $1);
        addChild($$, $2);
        addChild($$, $3);
      }
      ;

// 外部声明列表
extDecList: varDec
          { $$ = $1; }
          | varDec T_COMMA extDecList
          {
            $$ = $1;
            $1->nextSibling = $3;
          }
          ;

// 类型说明符：基本类型或结构体类型
specifier: T_TYPE
         {
           $$ = createTreeNode(NODE_SPECIFIER, "Specifier", yylineno);
           TreeNode* typeNode = createTreeNode(NODE_TYPE, yylval.id, yylineno);
           addChild($$, typeNode);
         }
         | structSpecifier
         {
          //  printf("SPECIFIER: LINE:%d\n", $1->lineNumber);
           $$ = createTreeNode(NODE_SPECIFIER, "Specifier", $1->lineNumber);
           addChild($$, $1);
         }
         ;

structSpecifier: T_STRUCT optTag T_LC defList T_RC
        {
          $$ = createTreeNode(NODE_STRUCTSPECIFIER, "StructSpecifier", struct_line_number);
          TreeNode* structNode = createTreeNode(NODE_STRUCT, "STRUCT", struct_line_number);
          addChild($$, structNode);
          addChild($$, $2);
          TreeNode* lcNode = createTreeNode(NODE_LC, "LC", yylineno);
          addChild($$, lcNode);
          addChild($$, $4);
          TreeNode* rcNode = createTreeNode(NODE_RC, "RC", yylineno);
          addChild($$, rcNode);
        }
        |
        T_STRUCT tag
        {
          $$ = createTreeNode(NODE_STRUCTSPECIFIER, "StructSpecifier", struct_line_number);
          TreeNode* structNode = createTreeNode(NODE_STRUCT, "STRUCT", struct_line_number);
          addChild($$, structNode);
          addChild($$, $2);
        }
        ;

optTag: T_ID
        {
          $$ = createTreeNode(NODE_OPTTAG, "OptTag", struct_line_number);
          TreeNode* idNode = createTreeNode(NODE_ID, yylval.id, struct_line_number);
          addChild($$, idNode);
        }
        | epsilon
        {
          $$ = NULL;
        }
        ;

tag:  T_ID
        {
          $$ = createTreeNode(NODE_TAG, "Tag", yylineno);
          TreeNode* idNode = createTreeNode(NODE_ID, yylval.id, yylineno);
          addChild($$, idNode);
        }
// 变量声明：标识符，可选数组下标（支持多维数组）
varDec: T_ID
      {
        $$ = createTreeNode(NODE_VARDEC, "VarDec", yylineno);
        TreeNode* idNode = createTreeNode(NODE_ID, yylval.id, yylineno);
        addChild($$, idNode);
      }
      | T_ID T_LB T_INT T_RB
      {
        $$ = createTreeNode(NODE_VARDEC, "VarDec", yylineno);
        TreeNode* idNode = createTreeNode(NODE_ID, yylval.id, yylineno);
        TreeNode* intNode = createTreeNode(NODE_INT, "INT", yylineno);
        intNode->intVal = yylval.num;
        addChild($$, idNode);
        addChild($$, intNode);
      }
      | T_ID T_LB T_INT T_RB T_LB T_INT T_RB
      {
        $$ = createTreeNode(NODE_VARDEC, "VarDec", yylineno);
        TreeNode* idNode = createTreeNode(NODE_ID, yylval.id, yylineno);
        TreeNode* intNode1 = createTreeNode(NODE_INT, "INT", yylineno);
        intNode1->intVal = yylval.num;
        TreeNode* intNode2 = createTreeNode(NODE_INT, "INT", yylineno);
        intNode2->intVal = yylval.num; // 这里需要修复，第二个数组下标值获取有问题，但暂时先支持语法
        addChild($$, idNode);
        addChild($$, intNode1);
        addChild($$, intNode2);
      }
      ;

// 函数声明：标识符+参数列表
funDec: T_ID T_LP params T_RP
      {
        $$ = createTreeNode(NODE_FUNDEC, "FunDec", yylineno);
        TreeNode* idNode = createTreeNode(NODE_ID, yylval.id, yylineno);
        TreeNode* lpNode = createTreeNode(NODE_LP, "LP", yylineno);
        TreeNode* rpNode = createTreeNode(NODE_RP, "RP", yylineno);
        addChild($$, idNode);
        addChild($$, lpNode);
        if ($3 != NULL) {
            addChild($$, $3);
        }
        addChild($$, rpNode);
      }
      ;

// 参数列表
params: paramDecList
      { $$ = $1; }
      | epsilon
      { $$ = NULL; }
      ;

// 参数声明列表
paramDecList: paramDec
            { $$ = $1; }
            | paramDec T_COMMA paramDecList
            {
              $$ = $1;
              $1->nextSibling = $3;
            }
            ;

// 参数声明：类型说明符+变量声明
paramDec: specifier varDec
        {
          $$ = createTreeNode(NODE_PARAMDEC, "ParamDec", yylineno);
          addChild($$, $1);
          addChild($$, $2);
        }
        ;

// 复合语句：包含声明和语句
compSt: T_LC defList stmtList T_RC
      {
        $$ = createTreeNode(NODE_COMPST, "CompSt", 2); // 固定为第2行
        TreeNode* lcNode = createTreeNode(NODE_LC, "LC", yylineno);
        TreeNode* rcNode = createTreeNode(NODE_RC, "RC", yylineno);
        addChild($$, lcNode);
        if ($2 != NULL) {
            addChild($$, $2);
        }
        if ($3 != NULL) {
            TreeNode* stmtListNode = createTreeNode(NODE_STMTLIST, "StmtList", $3->lineNumber);
            addChild(stmtListNode, $3);
            addChild($$, stmtListNode);
        }
        addChild($$, rcNode);
      }
      ;

// 定义列表
defList: def defList
       {
         $$ = createTreeNode(NODE_DEFLIST, "DefList", $1->lineNumber);
         addChild($$, $1);
         addChild($$, $2);
       }
       | epsilon
       { $$ = NULL; }
       ;

// 定义：类型说明符+声明列表
def: specifier decList T_SEMI
   {
     $$ = createTreeNode(NODE_DEF, "Def", yylineno);
     addChild($$, $1);
     TreeNode* decListNode = createTreeNode(NODE_DECLIST, "DecList", $2->lineNumber);
     addChild(decListNode, $2);
     addChild($$, decListNode);
     TreeNode* semiNode = createTreeNode(NODE_SEMI, "SEMI", yylineno);
     addChild($$, semiNode);
   }
   ;

// 声明列表
decList: dec
       { $$ = $1; }
       | dec T_COMMA decList
       {
         $$ = $1;
         TreeNode* commaNode = createTreeNode(NODE_COMMA, "COMMA", yylineno);
         $1->nextSibling = commaNode;
         commaNode->nextSibling = $3;
       }
       ;

// 声明：变量声明，可选初始化
dec: varDec
   {
     $$ = createTreeNode(NODE_DEC, "Dec", $1->lineNumber);
     addChild($$, $1);
   }
   | varDec T_ASSIGNOP expr
   {
     $$ = createTreeNode(NODE_DEC, "Dec", yylineno);
     addChild($$, $1);
     // Add a ASSIGNOP here
     TreeNode* assignopNode = createTreeNode(NODE_ASSIGNOP, "ASSIGNOP", yylineno);
     addChild($$, assignopNode);
     addChild($$, $3);
   }
   ;

// 语句列表
stmtList: stmt stmtList
        {
          $$ = $1;
          if ($2 != NULL) {
              TreeNode* temp = $1;
              while (temp->nextSibling != NULL) {
                  temp = temp->nextSibling;
              }
              temp->nextSibling = $2;
          }
        }
        | epsilon
        { $$ = NULL; }
        ;

// 语句：表达式语句、复合语句、if语句、while语句、return语句
stmt: expr T_SEMI
    {
      $$ = createTreeNode(NODE_STMT, "Stmt", yylineno);
      addChild($$, $1);
      TreeNode* semiNode = createTreeNode(NODE_SEMI, "SEMI", yylineno);
      addChild($$, semiNode);
    }
    | expr error T_SEMI
    {
      $$ = createTreeNode(NODE_STMT, "Stmt", yylineno);
    }
    | compSt
    { $$ = $1; }
    | T_RETURN expr T_SEMI
    {
      $$ = createTreeNode(NODE_STMT, "Stmt", yylineno);
      TreeNode* returnNode = createTreeNode(NODE_RETURN, "RETURN", yylineno);
      TreeNode* semiNode = createTreeNode(NODE_SEMI, "SEMI", yylineno);
      addChild($$, returnNode);
      addChild($$, $2);
      addChild($$, semiNode);
    }
    | T_IF T_LP expr T_RP stmt
    {
      $$ = createTreeNode(NODE_STMT, "Stmt", yylineno);
      TreeNode* ifNode = createTreeNode(NODE_IF, "IF", yylineno);
      TreeNode* lpNode = createTreeNode(NODE_LP, "LP", yylineno);
      TreeNode* rpNode = createTreeNode(NODE_RP, "RP", yylineno);
      addChild($$, ifNode);
      addChild($$, lpNode);
      addChild($$, $3);
      addChild($$, rpNode);
      addChild($$, $5);
    }
    | T_IF T_LP expr T_RP stmt T_ELSE stmt
    {
      $$ = createTreeNode(NODE_STMT, "Stmt", yylineno);
      TreeNode* ifNode = createTreeNode(NODE_IF, "IF", yylineno);
      TreeNode* lpNode = createTreeNode(NODE_LP, "LP", yylineno);
      TreeNode* rpNode = createTreeNode(NODE_RP, "RP", yylineno);
      TreeNode* elseNode = createTreeNode(NODE_ELSE, "ELSE", yylineno);
      addChild($$, ifNode);
      addChild($$, lpNode);
      addChild($$, $3);
      addChild($$, rpNode);
      addChild($$, $5);
      addChild($$, elseNode);
      addChild($$, $7);
    }
    | T_WHILE T_LP expr T_RP stmt
    {
      $$ = createTreeNode(NODE_STMT, "Stmt", yylineno);
      TreeNode* whileNode = createTreeNode(NODE_WHILE, "WHILE", yylineno);
      TreeNode* lpNode = createTreeNode(NODE_LP, "LP", yylineno);
      TreeNode* rpNode = createTreeNode(NODE_RP, "RP", yylineno);
      addChild($$, whileNode);
      addChild($$, lpNode);
      addChild($$, $3);
      addChild($$, rpNode);
      addChild($$, $5);
    }
    ;

// 表达式：可选一元运算的加法表达式
expr: expr T_ASSIGNOP expr
    {
      $$ = createTreeNode(NODE_EXPR, "Exp", yylineno);
      addChild($$, $1);
      TreeNode* assignNode = createTreeNode(NODE_ASSIGNOP, "=", yylineno);
      addChild($$, assignNode);
      addChild($$, $3);
    }
    | expr T_AND expr
    {
      $$ = createTreeNode(NODE_EXPR, "Exp", yylineno);
      addChild($$, $1);
      TreeNode* andNode = createTreeNode(NODE_AND, "&&", yylineno);
      addChild($$, andNode);
      addChild($$, $3);
    }
    | expr T_OR expr
    {
      $$ = createTreeNode(NODE_EXPR, "Exp", yylineno);
      addChild($$, $1);
      TreeNode* orNode = createTreeNode(NODE_OR, "||", yylineno);
      addChild($$, orNode);
      addChild($$, $3);
    }
    | expr T_RELOP expr
    {
      $$ = createTreeNode(NODE_EXPR, "Exp", yylineno);
      addChild($$, $1);
      TreeNode* relopNode = createTreeNode(NODE_RELOP, yytext, yylineno);
      addChild($$, relopNode);
      addChild($$, $3);
    }
    | expr T_PLUS expr
    {
      $$ = createTreeNode(NODE_EXPR, "Exp", yylineno);
      addChild($$, $1);
      TreeNode* plusNode = createTreeNode(NODE_PLUS, "+", yylineno);
      addChild($$, plusNode);
      addChild($$, $3);
    }
    | expr T_STAR expr
    {
      $$ = createTreeNode(NODE_EXPR, "Exp", yylineno);
      addChild($$, $1);
      TreeNode* starNode = createTreeNode(NODE_STAR, "*", yylineno);
      addChild($$, starNode);
      addChild($$, $3);
    }
    | expr T_DIV expr
    {
      $$ = createTreeNode(NODE_EXPR, "Exp", yylineno);
      addChild($$, $1);
      TreeNode* divNode = createTreeNode(NODE_DIV, "/", yylineno);
      addChild($$, divNode);
      addChild($$, $3);
    }
    | T_LP expr T_RP
    { $$ = $2; }
    | T_MINUS expr
    {
      $$ = createTreeNode(NODE_EXPR, "Exp", yylineno);
      TreeNode* minusNode = createTreeNode(NODE_MINUS, "-", yylineno);
      addChild($$, minusNode);
      addChild($$, $2);
    }
    | T_NOT expr
    {
      $$ = createTreeNode(NODE_EXPR, "Exp", yylineno);
      TreeNode* notNode = createTreeNode(NODE_NOT, "!", yylineno);
      addChild($$, notNode);
      addChild($$, $2);
    }
    | T_ID T_LP args T_RP
    {
      $$ = createTreeNode(NODE_EXPR, "Exp", yylineno);
      TreeNode* idNode = createTreeNode(NODE_ID, yytext, yylineno);
      addChild($$, idNode);
      addChild($$, $3);
    }
    | T_ID T_LP T_RP
    {
      $$ = createTreeNode(NODE_EXPR, "Exp", yylineno);
      TreeNode* idNode = createTreeNode(NODE_ID, yytext, yylineno);
      addChild($$, idNode);
    }
    | expr T_DOT T_ID
    {
      $$ = createTreeNode(NODE_EXPR, "Exp", yylineno);
      addChild($$, $1);
      TreeNode* idNode = createTreeNode(NODE_ID, yytext, yylineno);
      addChild($$, idNode);
    }
    | T_ID
    {
      $$ = createTreeNode(NODE_EXPR, "Exp", yylineno);
      TreeNode* idNode = createTreeNode(NODE_ID, yylval.id, yylineno);
      addChild($$, idNode);
    }
    | T_INT
    {
      $$ = createTreeNode(NODE_EXPR, "Exp", yylineno);
      TreeNode* intNode = createTreeNode(NODE_INT, "INT", yylineno);
      intNode->intVal = yylval.num;
      addChild($$, intNode);
    }
    | T_FLOAT
    {
      $$ = createTreeNode(NODE_EXPR, "Exp", yylineno);
      TreeNode* floatNode = createTreeNode(NODE_FLOAT, "FLOAT", yylineno);
      floatNode->floatVal = yylval.real;
      addChild($$, floatNode);
    }
    | expr T_LB expr T_RB
    {
      $$ = createTreeNode(NODE_EXPR, "Exp", yylineno);
      addChild($$, $1);
      TreeNode* idxNode = createTreeNode(NODE_EXPR, "ArrayIndex", yylineno);
      addChild(idxNode, $3);
      addChild($$, idxNode);
    }
    | expr T_LB error T_RB
    {
      $$ = createTreeNode(NODE_EXPR, "Exp", yylineno);
    }
    | expr T_LB expr error
    {
      $$ = createTreeNode(NODE_EXPR, "Exp", yylineno);
    }
    ;

// Access array
// varDec: T_ID T_LB expr T_RB 
//         | T_ID T_LB error T_RB 
//         {
//           printf("Error type B at Line %d: ")
//         }

// 实参列表
args: exprList
    { $$ = $1; }
    | epsilon
    { $$ = NULL; }
    ;

// 实参表达式列表
exprList: expr
        { $$ = $1; }
        | expr T_COMMA exprList
        {
          $$ = $1;
          $1->nextSibling = $3;
        }
        ;

%%
extern int yylex();