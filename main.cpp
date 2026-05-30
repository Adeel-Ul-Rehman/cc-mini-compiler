#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "symbol_table.h"
#include "tac.h"
#include "interpreter.h"
#include "error_handler.h"
#include "optimizer.h"

extern FILE *yyin;
extern int yyparse();
extern ASTNode *program_root;
extern int semantic_errors;

SymbolTable *symtab;
ErrorList *error_list;

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
    error_list = error_list_create();
    semantic_errors = 0;
    
    int result = yyparse();
    
    if (error_list_has_errors(error_list)) {
        error_list_print(error_list);
    }
    
    if(result == 0 && !error_list_has_errors(error_list)) {
        printf("Parsing successful.\n");
        
        // Apply constant folding optimization
        optimize_ast(program_root);
        
        ast_print(program_root, 0);
        print_symbol_table(symtab);
        
        TacProgram *tac = tac_generate(program_root);
        tac_print(tac);
        interpreter_execute(tac);
        tac_free(tac);
    } else if (error_list_has_errors(error_list)) {
        printf("Parsing failed due to %d errors.\n", error_list->count);
    } else {
        printf("Parsing failed.\n");
    }

    if(yyin != stdin) fclose(yyin);
    destroy_symbol_table(symtab);
    error_list_free(error_list);
    ast_free(program_root);
    return 0;
}