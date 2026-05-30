#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "symbol_table.h"

extern FILE *yyin;
extern int yyparse();
extern ASTNode *program_root;
extern int semantic_errors;
SymbolTable *symtab;

int main(int argc, char **argv) {
    if(argc > 1) {
        yyin = fopen(argv[1], "r");
        if(!yyin) {
            perror("Cannot open file");
            return 1;
        }
    } else {
        yyin = stdin;
    }

    symtab = create_symbol_table();
    semantic_errors = 0;
    int result = yyparse();
    
    if(result == 0 && semantic_errors == 0) {
        printf("Parsing successful.\n");
        ast_print(program_root, 0);
        print_symbol_table(symtab);
    } else if (semantic_errors > 0) {
        printf("Parsing failed due to semantic errors.\n");
    } else {
        printf("Parsing failed.\n");
    }

    if(yyin != stdin) fclose(yyin);
    destroy_symbol_table(symtab);
    ast_free(program_root);
    return 0;
}