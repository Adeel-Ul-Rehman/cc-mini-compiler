#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "symbol_table.h"

extern FILE *yyin;
extern int yyparse();
extern ASTNode *program_root;
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
    int result = yyparse();
    if(result == 0) {
        printf("Parsing successful.\n");
        ast_print(program_root, 0);
        print_symbol_table(symtab);
    } else {
        printf("Parsing failed.\n");
    }

    if(yyin != stdin) fclose(yyin);
    destroy_symbol_table(symtab);
    ast_free(program_root);
    return 0;
}