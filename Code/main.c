#include <stdio.h>
#include "tree/tree.h"
#include "syntax.tab.h"
#include "semantics.h"

// Declare the AST root from syntax.y
extern TreeNode* ast_root;
extern int has_error;

int main(int argc, char**argv)
{
    if(argc<=1) return 1;
    FILE*f = fopen(argv[1], "r");
    if(!f){
        perror(argv[1]);
        return 1;
    }

    yyrestart(f);
    yyparse();

    // If no syntax errors, perform semantic analysis
    if (!has_error && ast_root) {
        analyzeSemantics(ast_root);
        // Clean up
        freeTree(ast_root);
        cleanupSemantics();
    }

    return 0;
}
