%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 注意：避免直接包含 lex.yy.c，因为它有 main 函数和 token 宏定义
// 我们将通过头文件包含 token 定义

// 外部声明
extern int yylineno;
extern char* yytext;
extern int yylex();

// 错误处理函数
int yyerror(const char* msg) {
    printf("Syntax error: %s at line %d, token: %s\n", msg, yylineno, yytext);
    return 0;
}
%}

// Token 定义（与 lexical.l 中保持一致）
// 使用不同的名称前缀以避免与 lexical.l 中的宏冲突
%token T_INT T_FLOAT T_ID T_SEMI T_COMMA T_ASSIGNOP T_RELOP T_PLUS T_MINUS T_STAR T_DIV
%token T_AND T_OR T_DOT T_NOT T_TYPE T_LP T_RP T_LB T_RB T_LC T_RC T_STRUCT T_RETURN T_IF T_ELSE T_WHILE

// 开始符号
%start program

%%

// 程序结构：由外部定义列表组成
program: extDefList
       ;

// 外部定义列表：可以是空的，或者由多个外部定义组成
extDefList: extDef extDefList
          | /* 空 */
          ;

// 外部定义：类型说明符 + 标识符 + 分号（简单声明）
extDef: specifier T_ID T_SEMI
      ;

// 类型说明符：int 或 float
specifier: T_TYPE
         ;

%%
extern int yylex();