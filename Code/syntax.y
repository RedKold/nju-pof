%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree/tree.h"
#include "lex.yy.c"

// int yylex();

// // include it;
// extern char* yytext;
// extern int yylineno;
int yyerror(const char*s);

// Declare global variables from lexical.l
extern int struct_line_number; // for T_STRUCT
extern int lc_line_number;     // for T_LC

extern int parse_ok;

// Global AST root for semantic analysis
TreeNode* ast_root = NULL;


int error_cnt=0;
int has_error = 0;
// 错误处理函数
int yyerror(const char* msg) {
    // 检查该行是否已经有词法错误，如果有，就不报告语法错误
    extern int line_has_error[];
    if (!line_has_error[yylineno]) {
        error_cnt++;
        has_error = 1;
        // 替换常见错误信息为更友好的提示
        if (strstr(msg, "T_SEMI") != NULL) {
            // 检查是否是匿名结构体声明导致的错误
            extern FILE* yyin;
            long pos = ftell(yyin);
            // 查找当前行的开头
            long line_start = pos;
            while (line_start > 0 && fgetc(yyin) != '\n') line_start--;
            if (line_start > 0) line_start++;
            fseek(yyin, line_start, SEEK_SET);

            char buffer[256];
            int i = 0;
            char c;
            while ((c = fgetc(yyin)) != '\n' && c != EOF && i < 255) {
                buffer[i++] = c;
            }
            buffer[i] = '\0';
            fseek(yyin, pos, SEEK_SET);

            if (strstr(buffer, "struct") && (strstr(buffer, "{") || strstr(msg, "unexpected T_SEMI"))) {
                printf("Error type B at Line %d: Anonymous struct declaration.\n", yylineno);
            } else {
                printf("Error type B at Line %d: Missing \";\"\n", yylineno);
            }
        } else if (strstr(msg, "T_RB") != NULL) {
            printf("Error type B at Line %d: Missing \"]\"\n", yylineno);
        } else if (strstr(msg, "T_COMMA") != NULL && strstr(msg, "unexpected") != NULL) {
            // 处理 a[5,3] 这种情况的错误
            printf("Error type B at Line %d: Missing \"]\"\n", yylineno);
        } else if (strstr(msg, "T_ELSE") != NULL && strstr(msg, "unexpected") != NULL) {
            // 处理 if 语句后缺少分号的错误
            printf("Error type B at Line %d: Missing \";\"\n", yylineno);
        } else if (strstr(msg, "T_FLOAT") != NULL && strstr(msg, "array") != NULL || strstr(msg, "T_FLOAT") != NULL && strstr(msg, "T_INT") != NULL) {
            // 数组维度必须是整数
            printf("Error type B at Line %d: Array dimension must be integer.\n", yylineno);
        } else if (strstr(msg, "trailing") != NULL && strstr(msg, "comma") != NULL) {
            // 函数参数中的尾随逗号
            printf("Error type B at Line %d: Trailing comma in function parameters.\n", yylineno);
        } else if (strstr(msg, "anonymous") != NULL && strstr(msg, "struct") != NULL) {
            // 匿名结构体声明
            printf("Error type B at Line %d: Anonymous struct declaration.\n", yylineno);
        } else if (strstr(msg, "array") != NULL && strstr(msg, "initialization") != NULL) {
            // 数组初始化不支持
            printf("Error type B at Line %d: Array initialization not supported.\n", yylineno);
        } else if (strstr(msg, "empty") != NULL && strstr(msg, "statement") != NULL) {
            // 空的while语句体
            printf("Error type B at Line %d: Empty while statement body.\n", yylineno);
        } else {
            // 其他错误
            if (strstr(msg, "unexpected T_FLOAT, expecting T_INT")) {
                printf("Error type B at Line %d: Array dimension must be integer.\n", yylineno);
            } else if (strstr(msg, "unexpected T_RP, expecting")) {
                // 检查是否是函数参数尾随逗号
                extern FILE* yyin;
                long pos = ftell(yyin);
                fseek(yyin, -2, SEEK_CUR);
                char c = fgetc(yyin);
                fseek(yyin, pos, SEEK_SET);
                if (c == ',') {
                    printf("Error type B at Line %d: Trailing comma in function parameters.\n", yylineno);
                } else {
                    printf("Error type B at Line %d: %s\n", yylineno, msg);
                }
            } else if (strstr(msg, "unexpected T_LC")) {
                // 可能是数组初始化
                printf("Error type B at Line %d: Array initialization not supported.\n", yylineno);
            } else if (strstr(msg, "unexpected T_SEMI")) {
                // 检查是否是while语句后的空体
                extern FILE* yyin;
                long pos = ftell(yyin);
                long search_pos = pos - 30; // 搜索前30个字符
                if (search_pos < 0) search_pos = 0;
                fseek(yyin, search_pos, SEEK_SET);

                char buffer[100];
                fread(buffer, 1, pos - search_pos, yyin);
                buffer[pos - search_pos] = '\0';
                fseek(yyin, pos, SEEK_SET);

                if (strstr(buffer, "while")) {
                    printf("Error type B at Line %d: Empty while statement body.\n", yylineno);
                } else {
                    printf("Error type B at Line %d: %s\n", yylineno, msg);
                }
            } else {
                printf("Error type B at Line %d: %s\n", yylineno, msg);
            }
        }
        line_has_error[yylineno] = 1; // 标记该行有错误
    }
    return 0;
}
%}

%define parse.error verbose

// Token 定义（与 lexical.l 中保持一致）
%token <num> T_INT
%token <real> T_FLOAT
%token <node> T_ID
%token T_SEMI T_COMMA T_ASSIGNOP T_RELOP T_PLUS T_MINUS T_STAR T_DIV
%token T_AND T_OR T_DOT T_NOT
%token <id> T_TYPE
%token T_LP T_RP T_LB T_RB T_LC T_RC
%token T_STRUCT T_RETURN T_IF T_ELSE T_WHILE
%token UMINUS  // 用于表示一元负号的优先级

// 定义优先级：用于一元负号和表达式操作符
%right T_ASSIGNOP
%left T_OR
%left T_AND
%left T_RELOP
%left T_PLUS T_MINUS
%left T_STAR T_DIV
%right T_NOT
%right UMINUS

// 语法树节点类型定义
%union {
    TreeNode* node;  // 语法树节点指针
    int num;
    float real;
    char id[32];
}

// 为每个非终结符和终结符指定类型
%type <node> program extDefList extDef extDecList specifier structSpecifier optTag  tag varDec funDec compSt stmtList stmt
%type <node> defList def decList dec expr args varList paramDec epsilon

// 开始符号
%start program

%%

// 空产生式定义
epsilon: /* empty */
       { $$ = NULL; }
       ;


// High-level Definitions

// 程序结构：由外部定义列表组成
program: extDefList
       {
         // we let TreeNote as a part of attribute
         $$ = createTreeNode(NODE_PROGRAM, "Program", 1); // 固定为第1行
         addChild($$, $1);
         // if has error, don't print it
        if(has_error == 0)
          printTree($$, 0);
         // Store AST root for semantic analysis instead of freeing
         ast_root = $$;
       }
       | error
       {
         // 程序级别的错误恢复
         has_error = 1;
         $$ = createTreeNode(NODE_PROGRAM, "Program", yylineno);
       }
       ;

// 外部定义列表：可以是空的，或者由多个外部定义组成
extDefList: extDef extDefList
          {
            $$ = createTreeNode(NODE_EXTDEF_LIST, "ExtDefList", $1->lineNumber);
            addChild($$, $1);
            addChild($$, $2);
          }
          | epsilon
          { $$ = NULL; }
          | error T_SEMI extDefList  // 同步到分号，然后继续解析
          {
            has_error = 1;
            $$ = $3;
          }
          | error T_RC extDefList  // 同步到右大括号，然后继续解析
          {
            has_error = 1;
            $$ = $3;
          }
          ;

// 外部定义：变量声明、类型声明或函数定义
extDef: specifier extDecList T_SEMI
      {
        $$ = createTreeNode(NODE_EXTDEF, "ExtDef", $1->lineNumber); // 固定为第1行
        addChild($$, $1);
        addChild($$, $2);
        TreeNode* semiNode = createTreeNode(NODE_SEMI, "SEMI", yylineno);
        addChild($$, semiNode);
      }
      | specifier T_SEMI
      {
        $$ = createTreeNode(NODE_EXTDEF, "ExtDef", $1->lineNumber); // 固定为第1行
        addChild($$, $1);
        TreeNode* semiNode = createTreeNode(NODE_SEMI, "SEMI", yylineno);
        addChild($$, semiNode);
      }
      | specifier funDec compSt
      {
        $$ = createTreeNode(NODE_EXTDEF, "ExtDef", $1->lineNumber); // 固定为第1行
        addChild($$, $1);
        addChild($$, $2);
        addChild($$, $3);
      }
      | error T_SEMI  // 外部定义级别的错误恢复
      {
        has_error = 1;
        $$ = NULL;
      }
      ;

// 外部声明列表
// extDecList 外部声明列表：确保在 structSpecifier: T_STRUCT tag 之后不会有 varDec
// 如果前面是 structSpecifier（T_STRUCT 后紧跟 tag，那么不应该有 varDec，因此 specifier 后应该是 SEMI）
extDecList: varDec
          {
            $$ = $1;
          }
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
          // 直接使用解析过程中实际获取到的标签值
          addChild($$, $2);
        }
        ;

optTag: T_ID
        {
          $$ = createTreeNode(NODE_OPTTAG, "OptTag", struct_line_number);
          addChild($$, $1);
        }
        | epsilon
        {
          $$ = NULL;
        }
        ;

tag:  T_ID
        {
          $$ = createTreeNode(NODE_TAG, "Tag", yylineno);
          addChild($$, $1);
        }
// 变量声明：标识符，可选数组下标（支持多维数组）
varDec: T_ID
      {
        $$ = createTreeNode(NODE_VARDEC, "VarDec", yylineno);
        addChild($$, $1);
      }
      | varDec T_LB T_INT T_RB
      {
        // 创建新的 VarDec 节点来包裹数组声明
        TreeNode* newVarDec = createTreeNode(NODE_VARDEC, "VarDec", yylineno);
        addChild(newVarDec, $1);

        TreeNode* lbNode = createTreeNode(NODE_LB, "LB", yylineno);
        addChild(newVarDec, lbNode);

        TreeNode* intNode = createTreeNode(NODE_INT, "INT", yylineno);
        intNode->intVal = yylval.num;
        addChild(newVarDec, intNode);

        TreeNode* rbNode = createTreeNode(NODE_RB, "RB", yylineno);
        addChild(newVarDec, rbNode);

        $$ = newVarDec;
      }
      // after is error part!
      | varDec T_LB T_FLOAT T_RB
      {
        printf("Error type B at Line %d: Array dimension must be integer.\n", yylineno);
        has_error = 1;

        TreeNode* newVarDec = createTreeNode(NODE_VARDEC, "VarDec", yylineno);
        addChild(newVarDec, $1);

        TreeNode* lbNode = createTreeNode(NODE_LB, "LB", yylineno);
        addChild(newVarDec, lbNode);

        TreeNode* floatNode = createTreeNode(NODE_FLOAT, "FLOAT", yylineno);
        floatNode->floatVal = yylval.real;
        addChild(newVarDec, floatNode);

        TreeNode* rbNode = createTreeNode(NODE_RB, "RB", yylineno);
        addChild(newVarDec, rbNode);

        $$ = newVarDec;
      }
      | varDec T_LB expr T_RB
      {
        if ($3->type != NODE_INT) {
          printf("Error type B at Line %d: Array dimension must be integer.\n", yylineno);
          has_error = 1;
        }

        TreeNode* newVarDec = createTreeNode(NODE_VARDEC, "VarDec", yylineno);
        addChild(newVarDec, $1);

        TreeNode* lbNode = createTreeNode(NODE_LB, "LB", yylineno);
        addChild(newVarDec, lbNode);

        addChild(newVarDec, $3);

        TreeNode* rbNode = createTreeNode(NODE_RB, "RB", yylineno);
        addChild(newVarDec, rbNode);

        $$ = newVarDec;
      }
      ;

// 函数声明：标识符+参数列表
funDec: T_ID T_LP varList T_RP
      {
        $$ = createTreeNode(NODE_FUNDEC, "FunDec", yylineno);
        addChild($$, $1);
        TreeNode* lpNode = createTreeNode(NODE_LP, "LP", yylineno);
        TreeNode* rpNode = createTreeNode(NODE_RP, "RP", yylineno);
        addChild($$, lpNode);
        if ($3 != NULL) {
            addChild($$, $3);
        }
        addChild($$, rpNode);
      }
      | T_ID T_LP T_RP
      {
        $$ = createTreeNode(NODE_FUNDEC, "FunDec", yylineno);
        addChild($$, $1);
        TreeNode* lpNode = createTreeNode(NODE_LP, "LP", yylineno);
        TreeNode* rpNode = createTreeNode(NODE_RP, "RP", yylineno);
        addChild($$, lpNode);
        addChild($$, rpNode);
      }
      ;

// 变量列表（函数参数列表）
varList: paramDec
      { $$ = $1; }
      | paramDec T_COMMA varList
      {
        $$ = $1;
        $1->nextSibling = $3;
      }
      | paramDec T_COMMA
      {
        printf("Error type B at Line %d: Trailing comma in function parameters.\n", yylineno);
        has_error = 1;
        $$ = $1;
      }
      | epsilon
      { $$ = NULL; }
      ;

// 参数声明：类型说明符+变量声明
paramDec: specifier varDec
        {
          $$ = createTreeNode(NODE_PARAMDEC, "ParamDec", yylineno);
          addChild($$, $1);
          addChild($$, $2);
        }
        ;


/*
----- STATEMENTS -----
*/


// 复合语句：包含声明和语句
compSt: T_LC defList stmtList T_RC
      {
        // 使用 @1 获取 T_LC 标记的行号信息
        TreeNode* lcNode = createTreeNode(NODE_LC, "LC", @1.first_line);
        $$ = createTreeNode(NODE_COMPST, "CompSt", @1.first_line); // 第T_LC的行号
        TreeNode* rcNode = createTreeNode(NODE_RC, "RC", @4.first_line);
        addChild($$, lcNode);
        if ($2 != NULL) {
            addChild($$, $2);
        }
        if ($3 != NULL) {
            addChild($$, $3);
        }
        addChild($$, rcNode);
      }
      ;


// 语句列表
stmtList: stmt stmtList
        {
          $$ = createTreeNode(NODE_STMTLIST, "StmtList", $1->lineNumber);
          if ($2 != NULL) {
              $1->nextSibling = $2;
              addChild($$,$1);
          } else {
            addChild($$,$1);
          }
        }
        | epsilon
        { $$ = NULL; }
        | error T_SEMI stmtList  // 同步到分号，然后继续解析
        {
          has_error = 0;
          $$ = $3;
        }
        | error T_RC stmtList  // 同步到右大括号，然后继续解析
        {
          has_error = 0;
          $$ = $3;
        }
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
      has_error = 0;
      $$ = createTreeNode(NODE_STMT, "Stmt", yylineno);
    }
    | error T_SEMI
    {
      has_error = 0;
      $$ = createTreeNode(NODE_STMT, "Stmt", yylineno);
      TreeNode* semiNode = createTreeNode(NODE_SEMI, "SEMI", yylineno);
      addChild($$, semiNode);
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
    // a error type!
    | T_WHILE T_LP expr T_RP T_SEMI
    {
      printf("Error type B at Line %d: Empty while statement body.\n", yylineno);
      has_error = 0;
      $$ = createTreeNode(NODE_STMT, "Stmt", yylineno);
      TreeNode* whileNode = createTreeNode(NODE_WHILE, "WHILE", yylineno);
      TreeNode* lpNode = createTreeNode(NODE_LP, "LP", yylineno);
      TreeNode* rpNode = createTreeNode(NODE_RP, "RP", yylineno);
      TreeNode* semiNode = createTreeNode(NODE_SEMI, "SEMI", yylineno);
      addChild($$, whileNode);
      addChild($$, lpNode);
      addChild($$, $3);
      addChild($$, rpNode);
      addChild($$, semiNode);
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

/*
----- LOCAL DEFINITIONS -----
*/

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
     addChild($$, $2);
     TreeNode* semiNode = createTreeNode(NODE_SEMI, "SEMI", yylineno);
     addChild($$, semiNode);
   }
   ;

// 声明列表
decList: dec
       { 
        $$ = createTreeNode(NODE_DECLIST, "DecList",yylineno);
        addChild($$,$1);
       }
       | dec T_COMMA decList
       {
         $$ = createTreeNode(NODE_DECLIST, "DecList",yylineno);
         TreeNode* commaNode = createTreeNode(NODE_COMMA, "COMMA", yylineno);
         addChild($$,$1);
         addChild($$,commaNode);
         addChild($$,$3);
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
     // 检查是否是数组初始化
     int isArray = 0;
     TreeNode* temp = $1;
     while (temp) {
       if (temp->type == NODE_INT) { // varDec 中的数组维度
         isArray = 1;
         break;
       }
       temp = temp->firstChild;
     }

     if (isArray) {
       printf("Error type B at Line %d: Array initialization not supported.\n", yylineno);
       has_error = 1;
       $$ = createTreeNode(NODE_DEC, "Dec", yylineno);
       addChild($$, $1);
     } else {
       $$ = createTreeNode(NODE_DEC, "Dec", yylineno);
       addChild($$, $1);
       TreeNode* assignopNode = createTreeNode(NODE_ASSIGNOP, "ASSIGNOP", yylineno);
       addChild($$, assignopNode);
       addChild($$, $3);
     }
   }
   ;



// 表达式：可选一元运算的加法表达式
expr: 
    expr T_ASSIGNOP expr
    {
      // 检查是否是数组初始化
      int is_array = 0;
      TreeNode* temp = $1;
      while (temp) {
        if (temp->firstChild && temp->firstChild->type == NODE_EXPR && temp->firstChild->firstChild && temp->firstChild->firstChild->type == NODE_INT) {
          is_array = 1; // 找到数组访问表达式
        }
        temp = temp->firstChild;
      }

      // 检查右手边是否是数组初始化 { ... }
      int is_init = 0;
      temp = $3;
      while (temp) {
        if (strstr(temp->name, "LC") || strstr(temp->name, "RC")) {
          is_init = 1; // 找到 LC 或 RC，可能是数组初始化
        }
        temp = temp->firstChild;
      }

      if (is_array || is_init) {
        printf("Error type B at Line %d: Array initialization not supported.\n", yylineno);
        $$ = NULL;
      } else {
        $$ = createTreeNode(NODE_EXPR, "Exp", yylineno);
        addChild($$, $1);
        TreeNode* assignNode = createTreeNode(NODE_ASSIGNOP, "=", yylineno);
        addChild($$, assignNode);
        addChild($$, $3);
      }
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
    | expr T_MINUS expr
    {
      $$ = createTreeNode(NODE_EXPR, "Exp", yylineno);
      addChild($$, $1);
      TreeNode* minusNode = createTreeNode(NODE_MINUS, "-", yylineno);
      addChild($$, minusNode);
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
    | 
    expr T_ASSIGNOP T_LC args T_RC
    {
      // 检查数组声明和初始化的匹配性
      // 首先获取数组声明的大小
      int declared_size = -1;
      TreeNode* temp = $1;

      // 找到数组维度
      while (temp) {
        if (temp->type == NODE_VARDEC) {
          // varDec可能包含数组维度
          TreeNode* child = temp->firstChild;
          while (child) {
            if (child->type == NODE_INT) {
              declared_size = child->intVal;
              break;
            }
            child = child->nextSibling;
          }
        }

        temp = temp->firstChild;
      }

      // 计算初始化元素的数量
      int initialized_count = 0;
      temp = $4;
      if (temp) {
        initialized_count = 1;
        while (temp->nextSibling) {
          initialized_count++;
          temp = temp->nextSibling;
        }
      }

      if (declared_size != -1 && declared_size != initialized_count) {
        printf("Error type B at Line %d: Array dimension %d does not match initialization size %d.\n",
               yylineno, declared_size, initialized_count);
      } else {
        printf("Error type B at Line %d: Array initialization not supported.\n", yylineno);
      }

      has_error = 1;
      $$ = NULL;
    }
    | expr T_ASSIGNOP T_LC T_RC
    {
      printf("Error type B at Line %d: Array initialization not supported.\n", yylineno);
      has_error = 1;
      $$ = NULL;
    }
    |  T_ID T_LP args T_RP
    {
      $$ = createTreeNode(NODE_EXPR, "Exp", yylineno);
      addChild($$, $1);
      TreeNode* lpNode = createTreeNode(NODE_LP, "LP", yylineno);
      TreeNode* rpNode = createTreeNode(NODE_RP, "RP", yylineno);
      addChild($$, lpNode);
      addChild($$, $3);
      addChild($$, rpNode);
    }
    | T_ID T_LP T_RP
    {
      $$ = createTreeNode(NODE_EXPR, "Exp", yylineno);
      addChild($$, $1);
      TreeNode* lpNode = createTreeNode(NODE_LP, "LP", yylineno);
      TreeNode* rpNode = createTreeNode(NODE_RP, "RP", yylineno);
      addChild($$, lpNode);
      addChild($$, rpNode);
    }
    | expr T_LB expr T_RB
    {
      $$ = createTreeNode(NODE_EXPR, "Exp", yylineno);
      addChild($$, $1);
      TreeNode* lbNode = createTreeNode(NODE_LB, "LB", yylineno);
      TreeNode* rbNode = createTreeNode(NODE_RB, "RB", yylineno);
      addChild($$,lbNode);
      addChild($$, $3);
      addChild($$,rbNode);
    }
    | expr T_DOT T_ID
    {
      $$ = createTreeNode(NODE_EXPR, "Exp", yylineno);
      addChild($$, $1);
      TreeNode* dotNode = createTreeNode(NODE_DOT, "DOT", yylineno);
      addChild($$, dotNode);
      addChild($$, $3);
    }
    | T_ID
    {
      $$ = createTreeNode(NODE_EXPR, "Exp", yylineno);
      addChild($$, $1);
    }
    |  T_INT
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
args: expr T_COMMA args
    {
      $$ = createTreeNode(NODE_ARGS, "Args", yylineno);
      addChild($$, $1);
      TreeNode* commaNode = createTreeNode(NODE_COMMA, "COMMA", yylineno);
      addChild($$, commaNode);
      addChild($$,$3);
    }
    | expr
    { 
      $$ = createTreeNode(NODE_ARGS, "Args", yylineno);
      addChild($$, $1);
    }
    | expr T_COMMA
    {
      printf("Error type B at Line %d: Trailing comma in function call arguments.\n", yylineno);
      has_error = 1;
      $$ = $1;
    }
    | error T_COMMA args  // 同步到逗号，然后继续解析
    {
      has_error = 1;
      $$ = $3;
    }
    | error T_RP  // 同步到右括号，然后结束
    {
      has_error = 1;
      $$ = NULL;
    }
    ;

%%
// Declare the AST root for use in main.c
TreeNode* ast_root;
extern int yylex();