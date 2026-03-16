%{
#include <stdio.h>
#include <string.h>

// Token types
#define KEYWORD 1
#define IDENTIFIER 2
#define NUMBER 3
#define OPERATOR 4
#define PUNCTUATION 5
%}

%%

"if"|"else"|"while"|"for"|"int"|"char"|"void"   { printf("KEYWORD: %s\n", yytext); return KEYWORD; }
[a-zA-Z_][a-zA-Z0-9_]*                         { printf("IDENTIFIER: %s\n", yytext); return IDENTIFIER; }
[0-9]+                                        { printf("NUMBER: %s\n", yytext); return NUMBER; }
[+\-*/=<>!]+                                  { printf("OPERATOR: %s\n", yytext); return OPERATOR; }
[;,\(\)\{\}]                                  { printf("PUNCTUATION: %s\n", yytext); return PUNCTUATION; }
[ \t\n]+                                      { /* Skip whitespace */ }
.                                             { printf("UNKNOWN: %s\n", yytext); }

%%

int yywrap() {
    return 1;
}

int main() {
    printf("FLEX Tokenizer Test\n");
    printf("-------------------\n");
    printf("Enter text (Ctrl+D to end):\n");

    int token;
    while ((token = yylex()) != 0) {
        // Token processing is done in the rules section
    }

    printf("\n-------------------\n");
    printf("Tokenization complete.\n");
    return 0;
}